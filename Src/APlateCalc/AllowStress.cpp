// AllowStress.cpp: implementation of the CAllowStress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "APlateCalc.h"
#include "APlateCalcStd.h"
#include "AllowStress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAllowStress::CAllowStress(long nTypeAllow, long nMaterial)
{
	m_nTypeAllow		= nTypeAllow;
	m_nMaterial			= nMaterial;
	m_nSize				= 0;
	m_nThickSize		= 0;
	m_dAreaWeb			= 0;	// 복부판의 총단면적(mm2)
	m_dAreaConc			= 0;	// 압축플랜지 총단면적(mm2)
	m_dDistFlangeFixed	= 0;	// 압축플랜지 고정점간이 거리(mm)
	m_dWidthFlange		= 0;	// 압축플랜지 폭(mm)
	m_dK				= 0;	// sqrt(3+Aw/2Ac)
	m_dLengthBuckling	= 0;
	m_dTurningRadius	= 0;
	m_dTurningRadius	= 0;
	m_dThick			= 0;
	m_dWidth			= 0;
	InitData();
}

CAllowStress::~CAllowStress()
{

}

void CAllowStress::InitData()
{
	double	dValue	= 0;
	if(m_nTypeAllow==ALLOW_SHEARSTRESS)
	{
		// dThick<=40.0
		InsertItem(THICK40BELOW, "SS400",		80.0);		InsertItem(THICK40BELOW, "SM400",		80.0);
		InsertItem(THICK40BELOW, "SMA400",		80.0);		InsertItem(THICK40BELOW, "SM490",		110.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	110.0);		InsertItem(THICK40BELOW, "SM490Y",		120.0);
		InsertItem(THICK40BELOW, "SM520",		120.0);		InsertItem(THICK40BELOW, "SMA490",		120.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	120.0);		InsertItem(THICK40BELOW, "SM570",		150.0);
		InsertItem(THICK40BELOW, "SMA570",		150.0);		InsertItem(THICK40BELOW, "SM570-TMC",	150.0);
		// 40 < Thick <= 75
		InsertItem(THICK75BELOW, "SS400",		75.0);		InsertItem(THICK75BELOW, "SM400",		75.0);
		InsertItem(THICK75BELOW, "SMA400",		75.0);		InsertItem(THICK75BELOW, "SM490",		100.0);
		InsertItem(THICK75BELOW, "SM490C-TMC",	110.0);		InsertItem(THICK75BELOW, "SM490Y",		115.0);
		InsertItem(THICK75BELOW, "SM520",		115.0);		InsertItem(THICK75BELOW, "SMA490",		115.0);
		InsertItem(THICK75BELOW, "SM520C-TMC",	120.0);		InsertItem(THICK75BELOW, "SM570",		145.0);
		InsertItem(THICK75BELOW, "SMA570",		145.0);		InsertItem(THICK75BELOW, "SM570-TMC",	145.0);
		// 75 < Thick <= 100
		InsertItem(THICK100BELOW, "SS400",		75.0);		InsertItem(THICK100BELOW, "SM400",		75.0);
		InsertItem(THICK100BELOW, "SMA400",		75.0);		InsertItem(THICK100BELOW, "SM490",		100.0);
		InsertItem(THICK100BELOW, "SM490C-TMC",	110.0);		InsertItem(THICK100BELOW, "SM490Y",		110.0);
		InsertItem(THICK100BELOW, "SM520",		110.0);		InsertItem(THICK100BELOW, "SMA490",		110.0);
		InsertItem(THICK100BELOW, "SM520C-TMC",	120.0);		InsertItem(THICK100BELOW, "SM570",		140.0);
		InsertItem(THICK100BELOW, "SMA570",		140.0);		InsertItem(THICK100BELOW, "SM570-TMC",	150.0);
		// 포스코 추가 강종
		InsertItem(THICK40BELOW, "HSB500",	125);	InsertItem(THICK75BELOW, "HSB500",	125);	InsertItem(THICK100BELOW, "HSB500",	125);
		InsertItem(THICK40BELOW, "HSB600",	150);	InsertItem(THICK75BELOW, "HSB600",	150);	InsertItem(THICK100BELOW, "HSB600",	150);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_TENSIONSTRESS || m_nTypeAllow == ALLOW_COMPSTRESS)
	{
		// dThick<=40.0
		InsertItem(THICK40BELOW, "SS400",		140.0);		InsertItem(THICK40BELOW, "SM400",		140.0);
		InsertItem(THICK40BELOW, "SMA400",		140.0);		InsertItem(THICK40BELOW, "SM490",		190.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	190.0);		InsertItem(THICK40BELOW, "SM490Y",		210.0);
		InsertItem(THICK40BELOW, "SM520",		210.0);		InsertItem(THICK40BELOW, "SMA490",		210.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	210.0);		InsertItem(THICK40BELOW, "SM570",		260.0);
		InsertItem(THICK40BELOW, "SMA570",		260.0);		InsertItem(THICK40BELOW, "SM570-TMC",	260.0);
		// 40 < Thick <= 75
		InsertItem(THICK75BELOW, "SS400",		130.0);		InsertItem(THICK75BELOW, "SM400",		130.0);
		InsertItem(THICK75BELOW, "SMA400",		130.0);		InsertItem(THICK75BELOW, "SM490",		175.0);
		InsertItem(THICK75BELOW, "SM490C-TMC",	190.0);		InsertItem(THICK75BELOW, "SM490Y",		200.0);
		InsertItem(THICK75BELOW, "SM520",		200.0);		InsertItem(THICK75BELOW, "SMA490",		200.0);
		InsertItem(THICK75BELOW, "SM520C-TMC",	210.0);		InsertItem(THICK75BELOW, "SM570",		250.0);
		InsertItem(THICK75BELOW, "SMA570",		250.0);		InsertItem(THICK75BELOW, "SM570-TMC",	260.0);
		// 75 < Thick <= 100
		InsertItem(THICK100BELOW, "SS400",		130.0);		InsertItem(THICK100BELOW, "SM400",		130.0);
		InsertItem(THICK100BELOW, "SMA400",		130.0);		InsertItem(THICK100BELOW, "SM490",		175.0);
		InsertItem(THICK100BELOW, "SM490C-TMC",	190.0);		InsertItem(THICK100BELOW, "SM490Y",		195.0);
		InsertItem(THICK100BELOW, "SM520",		195.0);		InsertItem(THICK100BELOW, "SMA490",		195.0);
		InsertItem(THICK100BELOW, "SM520C-TMC",	210.0);		InsertItem(THICK100BELOW, "SM570",		245.0);
		InsertItem(THICK100BELOW, "SMA570",		245.0);		InsertItem(THICK100BELOW, "SM570-TMC",	260.0);
		// 포스코 추가 강종
		InsertItem(THICK40BELOW, "HSB500",	220);	InsertItem(THICK75BELOW, "HSB500",	220);	InsertItem(THICK100BELOW, "HSB500",	220);
		InsertItem(THICK40BELOW, "HSB600",	260);	InsertItem(THICK75BELOW, "HSB600",	260);	InsertItem(THICK100BELOW, "HSB600",	260);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_BUCKLINGCOEFF)
	{
		// dThick<=40.0
		InsertItem(THICK40BELOW, "SS400",		13.1);		InsertItem(THICK40BELOW, "SM400",		13.1);
		InsertItem(THICK40BELOW, "SMA400",		13.1);		InsertItem(THICK40BELOW, "SM490",		11.2);
		InsertItem(THICK40BELOW, "SM490C-TMC",	11.2);		InsertItem(THICK40BELOW, "SM490Y",		10.7);
		InsertItem(THICK40BELOW, "SM520",		10.7);		InsertItem(THICK40BELOW, "SMA490",		10.7);
		InsertItem(THICK40BELOW, "SM520C-TMC",	10.7);		InsertItem(THICK40BELOW, "SM570",		9.6);
		InsertItem(THICK40BELOW, "SMA570",		9.6);		InsertItem(THICK40BELOW, "SM570-TMC",	9.6);
		// 40 < Thick <= 75
		InsertItem(THICK75BELOW, "SS400",		13.6);		InsertItem(THICK75BELOW, "SM400",		13.6);
		InsertItem(THICK75BELOW, "SMA400",		13.6);		InsertItem(THICK75BELOW, "SM490",		11.7);
		InsertItem(THICK75BELOW, "SM490C-TMC",	11.2);		InsertItem(THICK75BELOW, "SM490Y",		11.0);
		InsertItem(THICK75BELOW, "SM520",		11.1);		InsertItem(THICK75BELOW, "SMA490",		11.0);
		InsertItem(THICK75BELOW, "SM520C-TMC",	10.7);		InsertItem(THICK75BELOW, "SM570",		9.8);
		InsertItem(THICK75BELOW, "SMA570",		9.8);		InsertItem(THICK75BELOW, "SM570-TMC",	9.6);
		// 75 < Thick <= 100
		InsertItem(THICK100BELOW, "SS400",		13.6);		InsertItem(THICK100BELOW, "SM400",		13.6);
		InsertItem(THICK100BELOW, "SMA400",		13.6);		InsertItem(THICK100BELOW, "SM490",		11.7);
		InsertItem(THICK100BELOW, "SM490C-TMC",	11.2);		InsertItem(THICK100BELOW, "SM490Y",		11.1);
		InsertItem(THICK100BELOW, "SM520",		11.1);		InsertItem(THICK100BELOW, "SMA490",		11.1);
		InsertItem(THICK100BELOW, "SM520C-TMC",	10.7);		InsertItem(THICK100BELOW, "SM570",		9.9);
		InsertItem(THICK100BELOW, "SMA570",		9.9);		InsertItem(THICK100BELOW, "SM570-TMC",	9.6);
		// 포스코 추가 강종
		InsertItem(THICK40BELOW, "HSB500",	10.4);	InsertItem(THICK75BELOW, "HSB500",	10.4);	InsertItem(THICK100BELOW, "HSB500",	10.4);
		InsertItem(THICK40BELOW, "HSB600",	9.6);	InsertItem(THICK75BELOW, "HSB600",	9.6);	InsertItem(THICK100BELOW, "HSB600",	9.6);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_STDYIELDPOINT_UP || m_nTypeAllow==ALLOW_STDYIELDPOINT_LO|| m_nTypeAllow==ALLOW_STDYIELDPOINT_WEB)
	{
		// dThick<=40.0
		InsertItem(THICK40BELOW, "SS400",		235.0);		InsertItem(THICK40BELOW, "SM400",		235.0);
		InsertItem(THICK40BELOW, "SMA400",		235.0);		InsertItem(THICK40BELOW, "SM490",		315.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	315.0);		InsertItem(THICK40BELOW, "SM490Y",		355.0);
		InsertItem(THICK40BELOW, "SM520",		355.0);		InsertItem(THICK40BELOW, "SMA490",		355.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	355.0);		InsertItem(THICK40BELOW, "SM570",		450.0);
		InsertItem(THICK40BELOW, "SMA570",		450.0);		InsertItem(THICK40BELOW, "SM570-TMC",	450.0);
		// 40 < Thick <= 75
		InsertItem(THICK75BELOW, "SS400",		215.0);		InsertItem(THICK75BELOW, "SM400",		215.0);
		InsertItem(THICK75BELOW, "SMA400",		215.0);		InsertItem(THICK75BELOW, "SM490",		295.0);
		InsertItem(THICK75BELOW, "SM490C-TMC",	315.0);		InsertItem(THICK75BELOW, "SM490Y",		335.0);
		InsertItem(THICK75BELOW, "SM520",		335.0);		InsertItem(THICK75BELOW, "SMA490",		335.0);
		InsertItem(THICK75BELOW, "SM520C-TMC",	355.0);		InsertItem(THICK75BELOW, "SM570",		430.0);
		InsertItem(THICK75BELOW, "SMA570",		430.0);		InsertItem(THICK75BELOW, "SM570-TMC",	450.0);
		// 75 < Thick <= 100
		InsertItem(THICK100BELOW, "SS400",		215.0);		InsertItem(THICK100BELOW, "SM400",		215.0);
		InsertItem(THICK100BELOW, "SMA400",		215.0);		InsertItem(THICK100BELOW, "SM490",		295.0);
		InsertItem(THICK100BELOW, "SM490C-TMC",	315.0);		InsertItem(THICK100BELOW, "SM490Y",		325.0);
		InsertItem(THICK100BELOW, "SM520",		325.0);		InsertItem(THICK100BELOW, "SMA490",		325.0);
		InsertItem(THICK100BELOW, "SM520C-TMC",	355.0);		InsertItem(THICK100BELOW, "SM570",		420.0);
		InsertItem(THICK100BELOW, "SMA570",		420.0);		InsertItem(THICK100BELOW, "SM570-TMC",	450.0);
		// 포스코 추가 강종
		InsertItem(THICK40BELOW, "HSB500",	380);	InsertItem(THICK75BELOW, "HSB500",	380);	InsertItem(THICK100BELOW, "HSB500",	380);
		InsertItem(THICK40BELOW, "HSB600",	450);	InsertItem(THICK75BELOW, "HSB600",	450);	InsertItem(THICK100BELOW, "HSB600",	450);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_BENDINGCOMP_CASE1)
	{
		// dThick<=40.0
		InsertItem(THICK40BELOW, "SS400",		140.0);		InsertItem(THICK40BELOW, "SM400",		140.0);
		InsertItem(THICK40BELOW, "SMA400",		140.0);		InsertItem(THICK40BELOW, "SM490",		190.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	190.0);		InsertItem(THICK40BELOW, "SM490Y",		210.0);
		InsertItem(THICK40BELOW, "SM520",		210.0);		InsertItem(THICK40BELOW, "SMA490",		210.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	210.0);		InsertItem(THICK40BELOW, "SM570",		260.0);
		InsertItem(THICK40BELOW, "SMA570",		260.0);		InsertItem(THICK40BELOW, "SM570-TMC",	260.0);
		// 40 < Thick <= 75
		InsertItem(THICK75BELOW, "SS400",		130.0);		InsertItem(THICK75BELOW, "SM400",		130.0);
		InsertItem(THICK75BELOW, "SMA400",		130.0);		InsertItem(THICK75BELOW, "SM490",		175.0);
		InsertItem(THICK75BELOW, "SM490C-TMC",	160.0);		InsertItem(THICK75BELOW, "SM490Y",		200.0);
		InsertItem(THICK75BELOW, "SM520",		200.0);		InsertItem(THICK75BELOW, "SMA490",		200.0);
		InsertItem(THICK75BELOW, "SM520C-TMC",	210.0);		InsertItem(THICK75BELOW, "SM570",		250.0);
		InsertItem(THICK75BELOW, "SMA570",		250.0);		InsertItem(THICK75BELOW, "SM570-TMC",	260.0);
		// 75 < Thick <= 100
		InsertItem(THICK100BELOW, "SS400",		130.0);		InsertItem(THICK100BELOW, "SM400",		130.0);
		InsertItem(THICK100BELOW, "SMA400",		130.0);		InsertItem(THICK100BELOW, "SM490",		175.0);
		InsertItem(THICK100BELOW, "SM490C-TMC",	190.0);		InsertItem(THICK100BELOW, "SM490Y",		195.0);
		InsertItem(THICK100BELOW, "SM520",		195.0);		InsertItem(THICK100BELOW, "SMA490",		195.0);
		InsertItem(THICK100BELOW, "SM520C-TMC",	210.0);		InsertItem(THICK100BELOW, "SM570",		245.0);
		InsertItem(THICK100BELOW, "SMA570",		245.0);		InsertItem(THICK100BELOW, "SM570-TMC",	260.0);
		// 포스코 추가 강종
		InsertItem(THICK40BELOW, "HSB500",	220);	InsertItem(THICK75BELOW, "HSB500",	220);	InsertItem(THICK100BELOW, "HSB500",	220);
		InsertItem(THICK40BELOW, "HSB600",	260);	InsertItem(THICK75BELOW, "HSB600",	260);	InsertItem(THICK100BELOW, "HSB600",	260);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_BENDINGCOMP_CASE2)
	{
		if(m_dAreaWeb==0.0 || m_dAreaConc==0.0 || m_dDistFlangeFixed==0.0 || m_dWidthFlange==0.0)	return;
		double	dRate	= m_dDistFlangeFixed/m_dWidthFlange;
		double	dK		= sqrt(3+m_dAreaWeb/(2*m_dAreaConc));

		if(m_dAreaWeb/m_dAreaConc<=2.0)
		{
			// dThick<=40.0
			if(dRate<=4.5)							dValue = 140.0;
			else if(dRate>4.5 && dRate<=30.0)		dValue = 140.0-2.4*(dRate-4.5);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SS400",		dValue, 140.0, 4.5, 2.4, 30.0);
			InsertItem(THICK40BELOW, "SM400",		dValue, 140.0, 4.5, 2.4, 30.0);
			InsertItem(THICK40BELOW, "SMA400",		dValue, 140.0, 4.5, 2.4, 30.0);
			if(dRate<=4.0)							dValue = 190.0;
			else if(dRate>4.0 && dRate<=30.0)		dValue = 190.0-3.8*(dRate-4.0);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SM490",		dValue, 190.0, 4.0, 3.8, 30.0);
			InsertItem(THICK40BELOW, "SM490C-TMC",	dValue, 190.0, 4.0, 3.8, 30.0);
			if(dRate<=3.5)							dValue = 210.0;
			else if(dRate>3.5 && dRate<=27.0)		dValue = 210.0-4.4*(dRate-3.5);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SM490Y",		dValue, 210.0, 3.5, 4.4, 27.0);
			InsertItem(THICK40BELOW, "SM520",		dValue, 210.0, 3.5, 4.4, 27.0);
			InsertItem(THICK40BELOW, "SMA490",		dValue, 210.0, 3.5, 4.4, 27.0);
			InsertItem(THICK40BELOW, "SM520C-TMC",	dValue, 210.0, 3.5, 4.4, 27.0);
			if(dRate<=5.0)							dValue = 260.0;
			else if(dRate>5.0 && dRate<=25.0)		dValue = 260.0-6.6*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SM570",		dValue, 260.0, 5.0, 6.6, 25.0);
			InsertItem(THICK40BELOW, "SMA570",		dValue, 260.0, 5.0, 6.6, 25.0);
			InsertItem(THICK40BELOW, "SM570-TMC",	dValue, 260.0, 5.0, 6.6, 25.0);
			// 40 < Thick <= 75
			if(dRate<=5.0)							dValue = 130.0;
			else if(dRate>5.0 && dRate<=30.0)		dValue = 130.0-2.2*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SS400",		dValue, 130.0, 5.0, 2.2, 30.0);
			InsertItem(THICK75BELOW, "SM400",		dValue, 130.0, 5.0, 2.2, 30.0);
			InsertItem(THICK75BELOW, "SMA400",		dValue, 130.0, 5.0, 2.2, 30.0);
			if(dRate<=4.0)							dValue = 175.0;
			else if(dRate>4.0 && dRate<=30.0)		dValue = 175.0-3.5*(dRate-4.0);
			else									dValue  = 0;
			InsertItem(THICK75BELOW, "SM490",		dValue, 175.0, 4.0, 3.5, 30.0);
			if(dRate<=4.0)							dValue = 190.0;
			else if(dRate>4.0 && dRate<=30.0)		dValue = 190.0-3.8*(dRate-4.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM490C-TMC",	dValue, 190.0, 4.0, 3.8, 30.0);
			if(dRate<=4.0)							dValue = 200.0;
			else if(dRate>4.0 && dRate<=27.0)		dValue = 200.0-4.2*(dRate-4.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM490Y",		dValue, 200.0, 4.0, 4.2, 27.0);
			InsertItem(THICK75BELOW, "SM520",		dValue, 200.0, 4.0, 4.2, 27.0);
			InsertItem(THICK75BELOW, "SMA490",		dValue, 200.0, 4.0, 4.2, 27.0);
			if(dRate<=3.5)							dValue = 210.0;
			else if(dRate>3.5 && dRate<=27.0)		dValue = 210.0-4.4*(dRate-3.5);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM520C-TMC",	dValue, 210.0, 3.5, 4.4, 27.0);
			if(dRate<=5.0)							dValue = 250.0;
			else if(dRate>4.0 && dRate<=25.0)		dValue = 250.0-6.2*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM570",		dValue, 250.0, 5.0, 6.2, 25.0);
			InsertItem(THICK75BELOW, "SMA570",		dValue, 250.0, 5.0, 6.2, 25.0);
			if(dRate<=5.0)							dValue = 260.0;
			else if(dRate>5.0 && dRate<=25.0)		dValue = 260.0-6.6*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM570-TMC",	dValue, 260.0, 5.0, 6.6, 25.0);
			// 75 < Thick <= 100
			if(dRate<=5.0)							dValue = 130.0;
			else if(dRate>5.0 && dRate<=30.0)		dValue = 130.0-2.2*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW, "SS400",		dValue, 130.0, 5.0, 2.2, 30.0);
			InsertItem(THICK100BELOW, "SM400",		dValue, 130.0, 5.0, 2.2, 30.0);
			InsertItem(THICK100BELOW, "SMA400",		dValue, 130.0, 5.0, 2.2, 30.0);
			if(dRate<=4.0)							dValue = 175.0;
			else if(dRate>4.0 && dRate<=30.0)		dValue = 175.0-3.5*(dRate-4.0);
			else									dValue = 0;			
			InsertItem(THICK100BELOW, "SM490",		dValue, 175.0, 4.0, 3.5, 30.0);
			if(dRate<=4.0)							dValue = 190.0;
			else if(dRate>4.0 && dRate<=30.0)		dValue = 190.0-3.8*(dRate-4.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW,"SM490C-TMC",	dValue, 190.0, 4.0, 3.8, 30.0);
			if(dRate<=4.0)							dValue = 195.0;
			else if(dRate>4.0 && dRate<=27.0)		dValue = 195.0-4.0*(dRate-4.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW, "SM490Y",		dValue, 195.0, 4.0, 4.0, 27.0);
			InsertItem(THICK100BELOW, "SM520",		dValue, 195.0, 4.0, 4.0, 27.0);
			InsertItem(THICK100BELOW, "SMA490",		dValue, 195.0, 4.0, 4.0, 27.0);
			if(dRate<=3.5)							dValue = 210.0;
			else if(dRate>3.5 && dRate<=27.0)		dValue = 210.0-4.4*(dRate-3.5);
			else									dValue = 0;
			InsertItem(THICK100BELOW,"SM520C-TMC",	dValue, 210.0, 3.5, 4.4, 27.0);
			if(dRate<=5.0)							dValue = 245.0;
			else if(dRate>5.0 && dRate<=25.0)		dValue = 245.0-6.0*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW, "SM570",		dValue, 245.0, 5.0, 6.0, 25.0);
			InsertItem(THICK100BELOW, "SMA570",		dValue, 245.0, 5.0, 6.0, 25.0);
			if(dRate<=5.0)							dValue = 260.0;
			else if(dRate>5.0 && dRate<=25.0)		dValue = 260.0-6.6*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW,"SM570-TMC",	dValue, 260.0, 5.0, 6.6, 25.0);
			// 포스코 추가 강종
			if(dRate<=3.7)							dValue = 220.0;
			else if(dRate>3.5 && dRate<=27.0)		dValue = 220.0-4.8*(dRate-3.7);
			else									dValue = 0;
			InsertItem(THICK40BELOW,"HSB500",		dValue, 220.0, 3.7, 4.8, 27.0);
			InsertItem(THICK75BELOW,"HSB500",		dValue, 220.0, 3.7, 4.8, 27.0);
			InsertItem(THICK100BELOW,"HSB500",		dValue, 220.0, 3.7, 4.8, 27.0);
			if(dRate<=5.0)							dValue = 260.0;
			else if(dRate>5.0 && dRate<=25.0)		dValue = 260.0-6.6*(dRate-5.0);
			else									dValue = 0;
			InsertItem(THICK40BELOW,"HSB600",		dValue, 260.0, 5.0, 6.6, 25.0);
			InsertItem(THICK75BELOW,"HSB600",		dValue, 260.0, 5.0, 6.6, 25.0);
			InsertItem(THICK100BELOW,"HSB600",		dValue, 260.0, 5.0, 6.6, 25.0);
		}
		else
		{
			// dThick<=40.0
			if(dRate<=9/dK)							dValue = 140.0;
			else if(dRate>9/dK && dRate<=30.0)		dValue = 140.0-1.2*(dK*dRate-9.0);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SS400",		dValue, 140.0, 9, 1.2, 30.0);
			InsertItem(THICK40BELOW, "SM400",		dValue, 140.0, 9, 1.2, 30.0);
			InsertItem(THICK40BELOW, "SMA400",		dValue, 140.0, 9, 1.2, 30.0);
			if(dRate<=8/dK)							dValue = 190.0;
			else if(dRate>8/dK && dRate<=30.0)		dValue = 190.0-1.9*(dK*dRate-8.0);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SM490",		dValue, 190.0, 8, 1.9, 30.0);
			InsertItem(THICK40BELOW, "SM490C-TMC",	dValue, 190.0, 8, 1.9, 30.0);
			if(dRate<=7/dK)							dValue = 210.0;
			else if(dRate>7/dK && dRate<=27.0)		dValue = 210.0-2.2*(dK*dRate-7.0);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SM490Y",		dValue, 210.0, 7, 2.2, 27.0);
			InsertItem(THICK40BELOW, "SM520",		dValue, 210.0, 7, 2.2, 27.0);
			InsertItem(THICK40BELOW, "SMA490",		dValue, 210.0, 7, 2.2, 27.0);
			InsertItem(THICK40BELOW, "SM520C-TMC",	dValue, 210.0, 7, 2.2, 27.0);
			if(dRate<=10/dK)						dValue = 260.0;
			else if(dRate>10/dK && dRate<=25.0)		dValue = 260.0-3.3*(dK*dRate-10.0);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "SM570",		dValue, 260.0, 10.0, 3.3, 25.0);
			InsertItem(THICK40BELOW, "SMA570",		dValue, 260.0, 10.0, 3.3, 25.0);
			InsertItem(THICK40BELOW, "SM570-TMC",	dValue, 260.0, 10.0, 3.3, 25.0);
			// 40 < Thick <= 75
			if(dRate<=10/dK)						dValue = 130.0;
			else if(dRate>10/dK && dRate<=30.0)		dValue = 130.0-1.1*(dK*dRate-10.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SS400",		dValue, 130.0, 10.0, 1.1, 30.0);
			InsertItem(THICK75BELOW, "SM400",		dValue, 130.0, 10.0, 1.1, 30.0);
			InsertItem(THICK75BELOW, "SMA400",		dValue, 130.0, 10.0, 1.1, 30.0);
			if(dRate<=8/dK)							dValue = 175.0;
			else if(dRate>8/dK && dRate<=30.0)		dValue = 175.0-1.75*(dK*dRate-8.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM490",		dValue, 175.0, 8, 1.75, 30.0);

			if(dRate<=8/dK)							dValue = 190.0;
			else if(dRate>8/dK && dRate<=30.0)		dValue = 190.0-1.9*(dK*dRate-8.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM490C-TMC",	dValue, 190.0, 8, 1.9, 30.0);
			if(dRate<=8/dK)							dValue = 200.0;
			else if(dRate>8/dK && dRate<=27.0)		dValue = 200.0-2.1*(dK*dRate-8.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM490Y",		dValue, 200.0, 8.0, 2.1, 27.0);
			InsertItem(THICK75BELOW, "SM520",		dValue, 200.0, 8.0, 2.1, 27.0);
			InsertItem(THICK75BELOW, "SMA490",		dValue, 200.0, 8.0, 2.1, 27.0);
			if(dRate<=7/dK)							dValue = 210.0;
			else if(dRate>7/dK && dRate<=27.0)		dValue = 210.0-2.2*(dK*dRate-7.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM520C-TMC",	dValue, 210.0, 7, 2.2, 27.0);
			if(dRate<=10/dK)						dValue = 250.0;
			else if(dRate>10/dK && dRate<=25.0)		dValue = 250.0-3.1*(dK*dRate-10.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM570",		dValue, 250.0, 10.0, 3.1, 25.0);
			InsertItem(THICK75BELOW, "SMA570",		dValue, 250.0, 10.0, 3.1, 25.0);
			if(dRate<=10/dK)						dValue = 260.0;
			else if(dRate>10/dK && dRate<=25.0)		dValue = 260.0-3.3*(dK*dRate-10.0);
			else									dValue = 0;
			InsertItem(THICK75BELOW, "SM570-TMC",	dValue, 260.0, 10.0, 3.3, 25.0);
			// 75 < Thick <= 100
			if(dRate<=10/dK)						dValue = 130.0;
			else if(dRate>10/dK && dRate<=30.0)		dValue = 130.0-1.1*(dK*dRate-10.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW, "SS400",		dValue, 130.0, 10.0, 1.1, 30.0);
			InsertItem(THICK100BELOW, "SM400",		dValue, 130.0, 10.0, 1.1, 30.0);
			InsertItem(THICK100BELOW, "SMA400",		dValue, 130.0, 10.0, 1.1, 30.0);
			if(dRate<=8/dK)							dValue = 175.0;
			else if(dRate>8/dK && dRate<=30.0)		dValue = 175.0-1.75*(dK*dRate-8.0);
			else									dValue = 0;		
			InsertItem(THICK100BELOW, "SM490",		dValue, 175.0, 8.0, 1.75, 30.0);
			if(dRate<=8/dK)							dValue = 190.0;
			else if(dRate>8/dK && dRate<=30.0)		dValue = 190.0-1.9*(dK*dRate-8.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW,"SM490C-TMC",	dValue, 190.0, 8, 1.9, 30.0);
			if(dRate<=8/dK)							dValue = 195.0;
			else if(dRate>8/dK && dRate<=27.0)		dValue = 195.0-2.0*(dK*dRate-8.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW, "SM490Y",		dValue, 175.0, 8.0, 2.0, 27.0);
			InsertItem(THICK100BELOW, "SM520",		dValue, 175.0, 8.0, 2.0, 27.0);
			InsertItem(THICK100BELOW, "SMA490",		dValue, 175.0, 8.0, 2.0, 27.0);
			if(dRate<=7/dK)							dValue = 210.0;
			else if(dRate>7/dK && dRate<=27.0)		dValue = 210.0-2.2*(dK*dRate-7.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW,"SM520C-TMC",	dValue, 210.0, 7, 2.2, 27.0);
			if(dRate<=10/dK)						dValue = 245.0;
			else if(dRate>10/dK && dRate<=25.0)		dValue = 245.0-3.0*(dK*dRate-10.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW, "SM570",		dValue, 245.0, 10, 3.0, 25.0);
			InsertItem(THICK100BELOW, "SMA570",		dValue, 245.0, 10, 3.0, 25.0);
			if(dRate<=10/dK)						dValue = 260.0;
			else if(dRate>10/dK && dRate<=25.0)		dValue = 260.0-3.3*(dK*dRate-10.0);
			else									dValue = 0;
			InsertItem(THICK100BELOW,"SM570-TMC",	dValue, 260.0, 10.0, 3.3, 25.0);
			// 포스코 추가 강종
			if(dRate<=7/dK)							dValue = 220.0;
			else if(dRate>7/dK && dRate<27.0)		dValue = 220.0 - 2.5*(dK*dRate-7);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "HSB500",		dValue, 220.0, 7.0, 2.5, 27.0);
			InsertItem(THICK75BELOW, "HSB500",		dValue, 220.0, 7.0, 2.5, 27.0);
			InsertItem(THICK100BELOW, "HSB500",		dValue, 220.0, 7.0, 2.5, 27.0);
			if(dRate<=10/dK)						dValue = 260.0;
			else if(dRate>10/dK && dRate<25.0)		dValue = 260.0 - 3.3*(dK*dRate-10);
			else									dValue = 0;
			InsertItem(THICK40BELOW, "HSB600",		dValue, 260.0, 10.0, 3.3, 25.0);
			InsertItem(THICK75BELOW, "HSB600",		dValue, 260.0, 10.0, 3.3, 25.0);
			InsertItem(THICK100BELOW, "HSB600",		dValue, 260.0, 10.0, 3.3, 25.0);
		}
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_BUCKLING_UNCONSIDER)
	{
		double	dRate	= m_dLengthBuckling/m_dTurningRadius;
		if(dRate<=20.0)							dValue	= 140.0;
		else if(dRate>20.0 && dRate<=93.0)		dValue	= 140.0-0.84*(dRate-20.0);
		else									dValue	= 1200000.0/(6700.0+pow(dRate,2));
		InsertItem(THICK40BELOW, "SS400",		dValue, 140.0, 20.0, 0.84, 93.0, 6700.0);
		InsertItem(THICK40BELOW, "SM400",		dValue, 140.0, 20.0, 0.84, 93.0, 6700.0);
		InsertItem(THICK40BELOW, "SMA400",		dValue, 140.0, 20.0, 0.84, 93.0, 6700.0);
		if(dRate<=15.0)							dValue	= 190.0;
		else if(dRate>15.0 && dRate<=80.0)		dValue	= 190.0-1.3*(dRate-15.0);
		else									dValue	= 1200000.0/(5000.0+pow(dRate,2));
		InsertItem(THICK40BELOW, "SM490",		dValue, 190.0, 15.0, 1.3, 80.0, 5000.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	dValue, 190.0, 15.0, 1.3, 80.0, 5000.0);
		if(dRate<=14.0)							dValue	= 210.0;
		else if(dRate>14.0 && dRate<=76.0)		dValue	= 210.0-1.5*(dRate-14.0);
		else									dValue	= 1200000.0/(4500.0+pow(dRate,2));
		InsertItem(THICK40BELOW, "SM490Y",		dValue, 210.0, 14.0, 1.5, 76.0, 4500.0);
		InsertItem(THICK40BELOW, "SM520",		dValue, 210.0, 14.0, 1.5, 76.0, 4500.0);
		InsertItem(THICK40BELOW, "SMA490",		dValue, 210.0, 14.0, 1.5, 76.0, 4500.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	dValue, 210.0, 14.0, 1.5, 76.0, 4500.0);
		if(dRate<=18.0)							dValue	= 260.0;
		else if(dRate>18.0 && dRate<=67.0)		dValue	= 260.0-2.2*(dRate-18.0);
		else									dValue	= 1200000.0/(3500.0+pow(dRate,2));
		InsertItem(THICK40BELOW, "SM570",		dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		InsertItem(THICK40BELOW, "SMA570",		dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		InsertItem(THICK40BELOW, "SM570-TMC",	dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		// 75
		if(dRate<=20.0)							dValue	= 130.0;
		else if(dRate>20.0 && dRate<=97.0)		dValue	= 130.0-0.75*(dRate-20.0);
		else									dValue	= 1200000.0/(7300.0+pow(dRate,2));
		InsertItem(THICK75BELOW, "SS400",		dValue, 130.0, 20.0, 0.75, 97.0, 7300.0);
		InsertItem(THICK75BELOW, "SM400",		dValue, 130.0, 20.0, 0.75, 97.0, 7300.0);
		InsertItem(THICK75BELOW, "SMA400",		dValue, 130.0, 20.0, 0.75, 97.0, 7300.0);
		if(dRate<=15.0)							dValue	= 175.0;
		else if(dRate>15.0 && dRate<=83.0)		dValue	= 175.0-1.1*(dRate-15.0);
		else									dValue	= 1200000.0/(5300.0+pow(dRate,2));
		InsertItem(THICK75BELOW, "SM490",		dValue, 175.0, 15.0, 1.1, 83.0, 5300.0);
		if(dRate<=15.0)							dValue	= 190.0;
		else if(dRate>15.0 && dRate<=80.0)		dValue	= 190.0-1.3*(dRate-15.0);
		else									dValue	= 1200000.0/(5000.0+pow(dRate,2));
		InsertItem(THICK75BELOW, "SM490C-TMC",	dValue, 190.0, 15.0, 1.3, 80.0, 5000.0);
		if(dRate<=14.0)							dValue	= 200.0;
		else if(dRate>14.0 && dRate<=78.0)		dValue	= 200.0-1.4*(dRate-14.0);
		else									dValue	= 1200000.0/(4700.0+pow(dRate,2));
		InsertItem(THICK75BELOW, "SM490Y",		dValue, 200.0, 14.0, 1.4, 78.0, 4700.0);
		InsertItem(THICK75BELOW, "SM520",		dValue, 200.0, 14.0, 1.4, 78.0, 4700.0);
		InsertItem(THICK75BELOW, "SMA490",		dValue, 200.0, 14.0, 1.4, 78.0, 4700.0);
		if(dRate<=14.0)							dValue	= 210.0;
		else if(dRate>14.0 && dRate<=76.0)		dValue	= 210.0-1.5*(dRate-14.0);
		else									dValue	= 1200000.0/(4500.0+pow(dRate,2));
		InsertItem(THICK75BELOW, "SM520C-TMC",	dValue, 210.0, 14.0, 1.5, 76.0, 4500.0);
		if(dRate<=18.0)							dValue	= 260.0;
		else if(dRate>18.0 && dRate<=67.0)		dValue	= 260.0-1.4*(dRate-18.0);
		else									dValue	= 1200000.0/(3500.0+pow(dRate,2));
		InsertItem(THICK75BELOW, "SM570",		dValue, 260.0, 18.0, 1.4, 67.0, 3500.0);
		InsertItem(THICK75BELOW, "SMA570",		dValue, 260.0, 18.0, 1.4, 67.0, 3500.0);
		if(dRate<=18.0)							dValue	= 260.0;
		else if(dRate>18.0 && dRate<=67.0)		dValue	= 260.0-2.2*(dRate-18.0);
		else									dValue	= 1200000.0/(3500.0+pow(dRate,2));
		InsertItem(THICK75BELOW, "SM570-TMC",	dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		// 100
		if(dRate<=20.0)							dValue	= 130.0;
		else if(dRate>20.0 && dRate<=67.0)		dValue	= 130.0-0.75*(dRate-20.0);
		else									dValue	= 1200000.0/(7300.0+pow(dRate,2));
		InsertItem(THICK100BELOW, "SS400",		dValue, 130.0, 20.0, 0.75, 67.0, 7300.0);
		InsertItem(THICK100BELOW, "SM400",		dValue, 130.0, 20.0, 0.75, 67.0, 7300.0);
		InsertItem(THICK100BELOW, "SMA400",		dValue, 130.0, 20.0, 0.75, 67.0, 7300.0);
		if(dRate<=15.0)							dValue	= 175.0;
		else if(dRate>15.0 && dRate<=83.0)		dValue	= 175.0-1.1*(dRate-15.0);
		else									dValue	= 1200000.0/(5300.0+pow(dRate,2));
		InsertItem(THICK100BELOW, "SM490",		dValue, 175.0, 15.0, 1.1, 83.0, 5300.0);
		if(dRate<=15.0)							dValue	= 190.0;
		else if(dRate>15.0 && dRate<=80.0)		dValue	= 190.0-1.3*(dRate-15.0);
		else									dValue	= 1200000.0/(5000.0+pow(dRate,2));
		InsertItem(THICK100BELOW,"SM490C-TMC",	dValue, 190.0, 15.0, 1.3, 80.0, 5000.0);
		if(dRate<=14.0)							dValue	= 195.0;
		else if(dRate>14.0 && dRate<=79.0)		dValue	= 195.0-1.3*(dRate-14.0);
		else									dValue	= 1200000.0/(4900.0+pow(dRate,2));
		InsertItem(THICK100BELOW, "SM490Y",		dValue, 195.0, 14.0, 1.3, 79.0, 4900.0);
		InsertItem(THICK100BELOW, "SM520",		dValue, 195.0, 14.0, 1.3, 79.0, 4900.0);
		InsertItem(THICK100BELOW, "SMA490",		dValue, 195.0, 14.0, 1.3, 79.0, 4900.0);
		if(dRate<=14.0)							dValue	= 210.0;
		else if(dRate>14.0 && dRate<=76.0)		dValue	= 210.0-1.5*(dRate-14.0);
		else									dValue	= 1200000.0/(4500.0+pow(dRate,2));
		InsertItem(THICK100BELOW,"SM520C-TMC",	dValue, 210.0, 14.0, 1.5, 76.0, 4500.0);
		if(dRate<=18.0)							dValue	= 245.0;
		else if(dRate>18.0 && dRate<=79.0)		dValue	= 245.0-2.2*(dRate-18.0);
		else									dValue	= 1200000.0/(3700.0+pow(dRate,2));
		InsertItem(THICK100BELOW, "SM570",		dValue, 245.0, 18.0, 2.2, 79.0, 3700.0);
		InsertItem(THICK100BELOW, "SMA570",		dValue, 245.0, 18.0, 2.2, 79.0, 3700.0);
		if(dRate<=18.0)							dValue	= 260.0;
		else if(dRate>18.0 && dRate<=67.0)		dValue	= 260.0-2.2*(dRate-18.0);
		else									dValue	= 1200000.0/(3500.0+pow(dRate,2));
		InsertItem(THICK100BELOW,"SM570-TMC",	dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		// 포스코 추가 강종
		if(dRate<=15.0)							dValue	= 220.0;
		else if(dRate>15.0 && dRate<=74.0)		dValue	= 220.0-1.6*(dRate-15.0);
		else									dValue	= 1200000.0/(4200.0+pow(dRate,2));
		InsertItem(THICK40BELOW,"HSB500",		dValue, 220.0, 15.0, 1.6, 74.0, 4200.0);
		InsertItem(THICK75BELOW,"HSB500",		dValue, 220.0, 15.0, 1.6, 74.0, 4200.0);
		InsertItem(THICK100BELOW,"HSB500",		dValue, 220.0, 15.0, 1.6, 74.0, 4200.0);
		if(dRate<=18.0)							dValue	= 260.0;
		else if(dRate>18.0 && dRate<=67.0)		dValue	= 260.0-2.2*(dRate-18.0);
		else									dValue	= 1200000.0/(3500.0+pow(dRate,2));
		InsertItem(THICK40BELOW,"HSB600",		dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		InsertItem(THICK75BELOW,"HSB600",		dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		InsertItem(THICK100BELOW,"HSB600",		dValue, 260.0, 18.0, 2.2, 67.0, 3500.0);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_BUCKLING)
	{
		double	t		= m_dThick;
		double	b		= m_dWidth;
		if(b/13.1<=t)							dValue	= 140.0;
		else if(b/16.0<=t && t<b/13.1)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK40BELOW, "SS400",		dValue, 140.0, 13.1, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SM400",		dValue, 140.0, 13.1, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SMA400",		dValue, 140.0, 13.1, 24000.0, 16.0);
		if(b/11.2<=t)							dValue	= 190.0;
		else if(b/16.0<=t && t<b/11.2)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK40BELOW, "SM490",		dValue, 140.0, 11.2, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	dValue, 140.0, 11.2, 24000.0, 16.0);
		if(b/10.7<=t)							dValue	= 210.0;
		else if(b/16.0<=t && t<b/10.7)			dValue	= 24000.0*pow(t/b,2.0);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK40BELOW, "SM490Y",		dValue, 210.0, 10.7, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SM520",		dValue, 210.0, 10.7, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SMA490",		dValue, 210.0, 10.7, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	dValue, 210.0, 10.7, 24000.0, 16.0);
		if(b/9.6<=t)							dValue	= 260.0;
		else if(b/16.0<=t && t<b/9.6)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK40BELOW, "SM570",		dValue, 260.0, 9.6, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SMA570",		dValue, 260.0, 9.6, 24000.0, 16.0);
		InsertItem(THICK40BELOW, "SM570-TMC",	dValue, 260.0, 9.6, 24000.0, 16.0);
		//
		if(b/13.6<=t)							dValue	= 130.0;
		else if(b/16.0<=t && t<b/13.6)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK75BELOW, "SS400",		dValue, 130.0, 13.6, 24000.0, 16.0);
		InsertItem(THICK75BELOW, "SM400",		dValue, 130.0, 13.6, 24000.0, 16.0);
		InsertItem(THICK75BELOW, "SMA400",		dValue, 130.0, 13.6, 24000.0, 16.0);
		if(b/11.7<=t)							dValue	= 175.0;
		else if(b/16.0<=t && t<b/11.7)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK75BELOW, "SM490",		dValue, 175.0, 11.7, 24000.0, 16.0);
		if(b/11.2<=t)							dValue	= 190.0;
		else if(b/16.0<=t && t<b/11.2)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK75BELOW, "SM490C-TMC",	dValue, 140.0, 11.2, 24000.0, 16.0);
		if(b/11.0<=t)							dValue	= 200.0;
		else if(b/16.0<=t && t<b/11.0)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK75BELOW, "SM490Y",		dValue, 200.0, 11.0, 24000.0, 16.0);
		InsertItem(THICK75BELOW, "SM520",		dValue, 200.0, 11.0, 24000.0, 16.0);
		InsertItem(THICK75BELOW, "SMA490",		dValue, 200.0, 11.0, 24000.0, 16.0);
		if(b/10.7<=t)							dValue	= 210.0;
		else if(b/16.0<=t && t<b/10.7)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK75BELOW, "SM520C-TMC",	dValue, 210.0, 10.7, 24000.0, 16.0);
		if(b/9.8<=t)							dValue	= 260.0;
		else if(b/16.0<=t && t<b/9.8)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK75BELOW, "SM570",		dValue, 260.0, 9.8, 24000.0, 16.0);
		InsertItem(THICK75BELOW, "SMA570",		dValue, 260.0, 9.8, 24000.0, 16.0);
		if(b/9.6<=t)							dValue	= 260.0;
		else if(b/16.0<=t && t<b/9.6)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK75BELOW, "SM570-TMC",	dValue, 260.0, 9.6, 24000.0, 16.0);
		//
		if(b/13.6<=t)							dValue	= 130.0;
		else if(b/16.0<=t && t<b/13.6)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK100BELOW, "SS400",		dValue, 130.0, 13.6, 24000.0, 16.0);
		InsertItem(THICK100BELOW, "SM400",		dValue, 130.0, 13.6, 24000.0, 16.0);
		InsertItem(THICK100BELOW, "SMA400",		dValue, 130.0, 13.6, 24000.0, 16.0);
		if(b/11.7<=t)							dValue	= 190.0;
		else if(b/16.0<=t && t<b/11.7)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK100BELOW, "SM490",		dValue, 190.0, 11.7, 24000.0, 16.0);
		if(b/11.2<=t)							dValue	= 190.0;
		else if(b/16.0<=t && t<b/11.2)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK100BELOW, "SM490C-TMC",	dValue, 140.0, 11.2, 24000.0, 16.0);
		if(b/11.1<=t)							dValue	= 195.0;
		else if(b/16.0<=t && t<b/11.1)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK100BELOW, "SM490Y",		dValue, 195.0, 11.1, 24000.0, 16.0);
		InsertItem(THICK100BELOW, "SM520",		dValue, 195.0, 11.1, 24000.0, 16.0);
		InsertItem(THICK100BELOW, "SMA490",		dValue, 195.0, 11.1, 24000.0, 16.0);
		if(b/10.7<=t)							dValue	= 210.0;
		else if(b/16.0<=t && t<b/10.7)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK100BELOW, "SM520C-TMC",	dValue, 210.0, 10.7, 24000.0, 16.0);
		if(b/9.9<=t)							dValue	= 245.0;
		else if(b/16.0<=t && t<b/9.9)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK100BELOW, "SM570",		dValue, 245.0, 9.9, 24000.0, 16.0);
		InsertItem(THICK100BELOW, "SMA570",		dValue, 245.0, 9.9, 24000.0, 16.0);
		if(b/9.6<=t)							dValue	= 260.0;
		else if(b/16.0<=t && t<b/9.6)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK100BELOW, "SM570-TMC",	dValue, 260.0, 9.6, 24000.0, 16.0);
		// 포스코 추가 강종
		if(b/10.3<=t)							dValue	= 220.0;
		else if(b/16.0<=t && t<b/10.3)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK40BELOW, "HSB500",		dValue, 220.0, 10.3, 24000.0, 16.0);
		InsertItem(THICK75BELOW, "HSB500",		dValue, 220.0, 10.3, 24000.0, 16.0);
		InsertItem(THICK100BELOW, "HSB500",		dValue, 220.0, 10.3, 24000.0, 16.0);
		if(b/9.6<=t)							dValue	= 260.0;
		else if(b/16.0<=t && t<b/9.6)			dValue	= 24000.0*pow(t/b,2);
		else									dValue	= 24000.0*pow(t/b,2);
		InsertItem(THICK40BELOW, "HSB600",		dValue, 260.0, 9.6, 24000.0, 16.0);
		InsertItem(THICK75BELOW, "HSB600",		dValue, 260.0, 9.6, 24000.0, 16.0);
		InsertItem(THICK100BELOW, "HSB600",		dValue, 260.0, 9.6, 24000.0, 16.0);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
	if(m_nTypeAllow==ALLOW_FLANGE_SPACE)
	{
		InsertItem(THICK40BELOW, "SS400",		70.0);		InsertItem(THICK40BELOW, "SM400",		70.0);
		InsertItem(THICK40BELOW, "SMA400",		70.0);		InsertItem(THICK40BELOW, "SM490",		60.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	60.0);		InsertItem(THICK40BELOW, "SM490Y",		57.0);
		InsertItem(THICK40BELOW, "SM520",		57.0);		InsertItem(THICK40BELOW, "SMA490",		57.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	57.0);		InsertItem(THICK40BELOW, "SM570",		50.0);
		InsertItem(THICK40BELOW, "SMA570",		50.0);		InsertItem(THICK40BELOW, "SM570-TMC",	50.0);
		// 포스코 추가 강종
		InsertItem(THICK40BELOW, "HSB500",	55.0);			InsertItem(THICK40BELOW, "HSB600",	50.0);
		m_nSize			= 14;
		m_nThickSize	= 1;
	}
	if(m_nTypeAllow==ALLOW_PLATEBRIDGE_WEBMINTHICK)
	{
		InsertItem(THICK40BELOW, "SS400",		152.0);		InsertItem(THICK40BELOW, "SM400",		152.0);
		InsertItem(THICK40BELOW, "SMA400",		152.0);		InsertItem(THICK40BELOW, "SM490",		130.0);
		InsertItem(THICK40BELOW, "SM490C-TMC",	130.0);		InsertItem(THICK40BELOW, "SM490Y",		123.0);
		InsertItem(THICK40BELOW, "SM520",		123.0);		InsertItem(THICK40BELOW, "SMA490",		123.0);
		InsertItem(THICK40BELOW, "SM520C-TMC",	123.0);		InsertItem(THICK40BELOW, "SM570",		110.0);
		InsertItem(THICK40BELOW, "SMA570",		110.0);		InsertItem(THICK40BELOW, "SM570-TMC",	110.0);

		InsertItem(THICK75BELOW, "SS400",		256.0);		InsertItem(THICK75BELOW, "SM400",		256.0);
		InsertItem(THICK75BELOW, "SMA400",		256.0);		InsertItem(THICK75BELOW, "SM490",		220.0);
		InsertItem(THICK75BELOW, "SM490C-TMC",	220.0);		InsertItem(THICK75BELOW, "SM490Y",		209.0);
		InsertItem(THICK75BELOW, "SM520",		209.0);		InsertItem(THICK75BELOW, "SMA490",		209.0);
		InsertItem(THICK75BELOW, "SM520C-TMC",	209.0);		InsertItem(THICK75BELOW, "SM570",		188.0);
		InsertItem(THICK75BELOW, "SMA570",		188.0);		InsertItem(THICK75BELOW, "SM570-TMC",	188.0);

		InsertItem(THICK100BELOW, "SS400",		310.0);		InsertItem(THICK100BELOW, "SM400",		310.0);
		InsertItem(THICK100BELOW, "SMA400",		310.0);		InsertItem(THICK100BELOW, "SM490",		310.0);
		InsertItem(THICK100BELOW, "SM490C-TMC",	310.0);		InsertItem(THICK100BELOW, "SM490Y",		294.0);
		InsertItem(THICK100BELOW, "SM520",		294.0);		InsertItem(THICK100BELOW, "SMA490",		294.0);
		InsertItem(THICK100BELOW, "SM520C-TMC",	294.0);		InsertItem(THICK100BELOW, "SM570",		262.0);
		InsertItem(THICK100BELOW, "SMA570",		262.0);		InsertItem(THICK100BELOW, "SM570-TMC",	262.0);
		// 포스코 추가 강종
		InsertItem(THICK40BELOW, "HSB500",	120.0);	InsertItem(THICK75BELOW, "HSB500",	205.0);	InsertItem(THICK100BELOW, "HSB500",	286.0);
		InsertItem(THICK40BELOW, "HSB600",	110.0);	InsertItem(THICK75BELOW, "HSB600",	188.0);	InsertItem(THICK100BELOW, "HSB600",	262.0);
		m_nSize			= 14;
		m_nThickSize	= 3;
	}
}

void CAllowStress::ClearStress()
{
	for(long n=0; n<ALLOW_QTY_MAX; n++)
		m_vtAllowStress[n].clear();
}

void CAllowStress::InsertItem(long nThickIdx, CString strMaterial, double dAllowStress, double dAllowCoeff1, double dAllowCoeff2, double dAllowCoeff3, double dAllowCoeff4, double dAllowCoeff5)
{
	ALLOWSTRESS	vtAllowStress;
	vtAllowStress.m_dAllowStress	= dAllowStress;
	vtAllowStress.m_strMaterial		= strMaterial;
	vtAllowStress.m_dCoeff1			= dAllowCoeff1;
	vtAllowStress.m_dCoeff2			= dAllowCoeff2;
	vtAllowStress.m_dCoeff3			= dAllowCoeff3;
	vtAllowStress.m_dCoeff4			= dAllowCoeff4;
	vtAllowStress.m_dCoeff5			= dAllowCoeff5;
	m_vtAllowStress[nThickIdx].push_back(vtAllowStress);
}

long CAllowStress::GetIdxByThick(double dThick)
{
	long	nIdx	= 0;

	if(m_nTypeAllow==ALLOW_PLATEBRIDGE_WEBMINTHICK)
	{
		if(dThick>2)	nIdx	= 2;
		else			nIdx	= (long)dThick;
	}
	else
	{
		if(dThick<=40.0)						nIdx = THICK40BELOW;
		else if(dThick>40.0 && dThick<= 75.0)	nIdx = THICK75BELOW;
		else if(dThick>75.0 && dThick<= 100.0)	nIdx = THICK100BELOW;
	}
	return nIdx;
}

double CAllowStress::GetAllowStress(CString strMaterial, double dThick)
{
	long	nIdx = GetIdxByThick(dThick);

	for(long n=0; n<m_vtAllowStress[nIdx].size(); n++)
	{
		CString	szMat = m_vtAllowStress[nIdx][n].m_strMaterial;
		if(m_vtAllowStress[nIdx][n].m_strMaterial == strMaterial)
			return m_vtAllowStress[nIdx][n].m_dAllowStress;
	}

	return	0;
}

void CAllowStress::SetAllowBendingStress(double dAw, double dAc, double dDistFlangeFixed, double dWidthFlange)
{
	m_dAreaWeb			= dAw;				m_dAreaConc		= dAc;
	m_dDistFlangeFixed	= dDistFlangeFixed;	m_dWidthFlange	= dWidthFlange;
	ClearStress();
	InitData();
}

void CAllowStress::SetAllowBucklingUnconsider(double dLengthBuckling, double dTurnigRadius)
{
	m_dLengthBuckling	= dLengthBuckling;
	m_dTurningRadius	= dTurnigRadius;
	ClearStress();
	InitData();
}

void CAllowStress::SetAllowBuckling(double dThick, double dWidth)
{
	m_dThick	= dThick;
	m_dWidth	= dWidth;
	ClearStress();
	InitData();
}

long CAllowStress::GetMaterialIndex(CString strMaterial)
{
	for(long n=0; n<m_vtAllowStress[0].size(); n++)
		if(m_vtAllowStress[0][n].m_strMaterial == strMaterial)	return n;

	return	0;
}

CString CAllowStress::GetAllowStressTitle()
{
	CString	strRef	= _T("");

	switch(m_nTypeAllow)
	{
		case ALLOW_SHEARSTRESS :
			strRef	= _T("허용전단응력 (MPa)  ( ☞ 도.설.기 표 3.3.5 )");
			break;
		case ALLOW_TENSIONSTRESS :
			strRef	=  _T("허용축방향인장응력 (MPa)  ( ☞ 도.설.기 표 3.3.1 )");
			break;
		case ALLOW_COMPSTRESS :
			strRef	=  _T("허용압축응력 (MPa)  ( ☞ 도.설.기 표 3.3.4(a) )");
			break;
		case ALLOW_BUCKLINGCOEFF :
			strRef	=  _T("자유돌출판의 국부좌굴에 대한 허용응력  ( ☞ 도.설.기 표 3.4.4 )");
			break;
		case ALLOW_STDYIELDPOINT_UP :
			strRef	=  _T("판두께에 따른 기준항복점 - 상판 (MPa) ( ☞ 도.설.기 표 3.2.3 )");
			break;
		case ALLOW_STDYIELDPOINT_LO :
			strRef	=  _T("판두께에 따른 기준항복점 - 하판 (MPa) ( ☞ 도.설.기 표 3.2.3 )");
			break;
		case ALLOW_STDYIELDPOINT_WEB :
			strRef	=  _T("판두께에 따른 기준항복점 - 복부판 (MPa) ( ☞ 도.설.기 표 3.2.3 )");
			break;
		case ALLOW_BUCKLING_UNCONSIDER :
			strRef	=  _T("국부좌굴을 고려하지 않은 허용압축응력 (MPa) ( ☞ 도.설.기 표 3.3.2 )");
			break;
		case ALLOW_FLANGE_SPACE :
			strRef	=  _T("수직보강재를 생략할 수 있는 플랜지 순간격의 최대치( ☞ 도.설.기 표 3.8.3 )");
			break;
	}
	return strRef;
}

long CAllowStress::GetDecimalCiphers()
{
	long nDecimal	= 0;
	switch(m_nTypeAllow)
	{
		case ALLOW_SHEARSTRESS :			nDecimal = 0;	break;
		case ALLOW_TENSIONSTRESS :			nDecimal = 0;	break;
		case ALLOW_COMPSTRESS :				nDecimal = 0;	break;
		case ALLOW_BUCKLINGCOEFF :			nDecimal = 1;	break;
		case ALLOW_STDYIELDPOINT_UP :		nDecimal = 0;	break;
		case ALLOW_STDYIELDPOINT_LO :		nDecimal = 0;	break;
		case ALLOW_BENDINGCOMP_CASE1 :		nDecimal = 0;	break;
		case ALLOW_BENDINGCOMP_CASE2 :		nDecimal = 1;	break;
		case ALLOW_BUCKLING_UNCONSIDER :	nDecimal = 1;	break;
		case ALLOW_BUCKLING :				nDecimal = 1;	break;
		case ALLOW_FLANGE_SPACE :			nDecimal = 0;	break;
	default:
		break;
	}
	return nDecimal;
}
