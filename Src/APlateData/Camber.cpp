// Camber.cpp: implementation of the CCamber class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "Camber.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamber::CCamber()
{
	m_dStationCamber		= 0.0;
	m_dCamberJongDan		= 0.0;
	m_dCamberPyungubae		= 0.0;
	m_dCamberSteelOnly		= 0.0;
	m_dCamberFixLoad		= 0.0;
	m_dCamberVividLoad		= 0.0;
	m_dCamberUser1			= 0.0;
	m_dCamberUser2			= 0.0;
	m_dCamberFactory		= 0.0;
	m_dCamberField			= 0.0;
	m_dCamberSteelRemove	= 0.0;
	m_dCamberVividRemove	= 0.0;
	m_dCamberConf			= 0.0;
	m_dCamberTurning		= 0.0;
	for(long n = 0; n < 10; n++)
	{
		m_User[n] = 0;
	}
}

CCamber::~CCamber()
{

}

void CCamber::Serialize(CArchive& ar)
{
	short nSelf = 0;
	short nFlag = 1;	// TU_GIRDER
	if( ar.IsStoring() )
	{
		ar << nSelf;
		ar << nFlag;
		ar << m_dStationCamber;
		ar << m_dCamberJongDan;
		ar << m_dCamberPyungubae;
		ar << m_dCamberSteelOnly;
		ar << m_dCamberFixLoad;
		ar << m_dCamberVividLoad;
		ar << m_dCamberUser1;
		ar << m_dCamberUser2;
		ar << m_dCamberFactory;
		ar << m_dCamberField;
		ar << m_dCamberSteelRemove;
		ar << m_dCamberVividRemove;
		for (long i = 0; i < 10; i++)
			ar << m_User[i];
		ar << m_dCamberConf;
		ar << m_dCamberTurning;
	}
	else
	{
		ar >> nSelf;
		ar >> nFlag; 
		ar >> m_dStationCamber;
		ar >> m_dCamberJongDan;
		ar >> m_dCamberPyungubae;
		ar >> m_dCamberSteelOnly;
		ar >> m_dCamberFixLoad;
		ar >> m_dCamberVividLoad;
		ar >> m_dCamberUser1;
		ar >> m_dCamberUser2;
		ar >> m_dCamberFactory;
		ar >> m_dCamberField;
		ar >> m_dCamberSteelRemove;
		ar >> m_dCamberVividRemove;
		for (long i = 0; i < 10; i++)
			ar >> m_User[i];
	
		m_dCamberUser1 = m_User[0];
		m_dCamberUser2 = m_User[1];

		if(nFlag>0)
		{
			ar >> m_dCamberConf;
			ar >> m_dCamberTurning;
		}
	}
}


void CCamber::SerializeForOldVersion(CArchive& ar)
{
	long nSelf = 0;
	if(!ar.IsStoring())
	{
		ar >> nSelf;
		ar >> m_dStationCamber;//m_Pos; 		
		ar >> m_dCamberJongDan;
		ar >> m_dCamberSteelOnly;
		ar >> m_dCamberFixLoad;
		ar >> m_dCamberVividLoad;
		ar >> m_dCamberFactory;
		ar >> m_dCamberField;
		ar >> m_dCamberSteelRemove;
		ar >> m_dCamberVividRemove;
	}
}

CCamber& CCamber::operator=(const CCamber& obj)
{
	m_dStationCamber		= obj.m_dStationCamber;
	m_dCamberJongDan		= obj.m_dCamberJongDan;
	m_dCamberPyungubae		= obj.m_dCamberPyungubae;
	m_dCamberSteelOnly		= obj.m_dCamberSteelOnly;
	m_dCamberFixLoad		= obj.m_dCamberFixLoad;
	m_dCamberVividLoad		= obj.m_dCamberVividLoad;
	m_dCamberUser1			= obj.m_dCamberUser1;
	m_dCamberUser2			= obj.m_dCamberUser2;
	m_dCamberFactory	    = obj.m_dCamberFactory;
	m_dCamberField			= obj.m_dCamberField;
	m_dCamberSteelRemove	= obj.m_dCamberSteelRemove;
	m_dCamberVividRemove	= obj.m_dCamberVividRemove;
	m_dCamberConf			= obj.m_dCamberConf;
	m_dCamberTurning		= obj.m_dCamberTurning;
	return *this;
}

//입력 : nType =
//			0. 종단곡선에 의한
//			1. 편경사에 의한
//			②강재 자중에 의한 솟음량
//			③합성전 1차사하중에 의한 솟음량
//			④합성후 2차사하중에 의한 솟음량
//			⑤사용자 조정량1 - 삭제 20060920 by Sim
//			⑥사용자 조정량2 - 삭제 20060920 by Sim
//			⑦공장 제작시(0~6의 합계)
//			⑧현장 거치시(7-②-5-6)
//			⑨1차사하중 재하시(8-③)
//			10. 2차사하중 재하시(9-④)
//			11. station
//		 nValue 세팅할 값
//기능 : 7.공장제작시~10.2차사하중 재하시에는 자체 계산하여 세팅(dValue값이 의미가 없음)
void CCamber::SetCamberByType(long nBridgeType, long nType, double dValue)
{
	if(nBridgeType==TU_GIR)	SetCamberByType_TUGirder(nType, dValue);
	else					SetCamberByType_GenGirder(nType, dValue);
}

void CCamber::SetCamberByType_GenGirder(long nType, double dValue)
{
	switch (nType) 
	{
		case 0 :
			m_dCamberJongDan = dValue;
			break;
		case 1 :
			m_dCamberPyungubae = dValue;
			break;
		case 2 :
			m_dCamberSteelOnly = dValue;
			break;
		case 3 :
			m_dCamberFixLoad = dValue;
			break;
		case 4 :
			m_dCamberVividLoad = dValue;
			break;
		case 5 :
			m_dCamberFactory = m_dCamberSteelOnly+m_dCamberFixLoad+m_dCamberVividLoad+m_dCamberUser1+m_dCamberUser2;
			break;
		case 6 :
			m_dCamberField = m_dCamberFixLoad+m_dCamberVividLoad;
			break;
		case 7 :
			m_dCamberSteelRemove = m_dCamberVividLoad;
			break;
		case 8 :
			m_dCamberVividRemove = 0;
			break;
	}
}

void CCamber::SetCamberByType_TUGirder(long nType, double dValue)
{
	switch (nType) 
	{
		case 0 :
			m_dCamberJongDan	= dValue;
			break;
		case 1 :
			m_dCamberPyungubae	= dValue;
			break;
		case 2 :
			m_dCamberSteelOnly	= dValue;
			break;
		case 3 :
			m_dCamberConf		= dValue;
			break;
		case 4 :
			m_dCamberTurning	= dValue;
			break;
		case 5 :
			m_dCamberFixLoad	= dValue;
			break;
		case 6 :
			m_dCamberVividLoad	= dValue;
			break;
		case 7 :
			m_dCamberFactory	= m_dCamberSteelOnly + m_dCamberConf+ m_dCamberTurning + m_dCamberFixLoad + m_dCamberVividLoad;
			break;
		case 8 :
			m_dCamberField		= m_dCamberFixLoad + m_dCamberVividLoad;
			break;
		case 9 :
			m_dCamberSteelRemove = m_dCamberVividLoad;
			break;
		case 10 :
			m_dCamberVividRemove = 0;
			break;
	}
}
//입력 : nType =
//			0. 종단곡선에 의한
//			1. 편경사에 의한
//			②강재 자중에 의한 솟음량
//			③합성전 1차사하중에 의한 솟음량
//			④합성후 2차사하중에 의한 솟음량
//			⑤사용자 조정량1 - 삭제 20060920 by Sim
//			⑥사용자 조정량2 - 삭제 20060920 by Sim
//			⑦공장 제작시(2~6의 합계)
//			⑧현장 거치시(7-②-5-6)
//			⑨1차사하중 재하시(8-③)
//			10. 2차사하중 재하시(9-④)
//			11. station
double CCamber::GetCamberByType(long nBridgeType, long nType)
{
	double	dValue = 0;;
	if(nBridgeType==TU_GIR)	dValue = GetCamberByType_TUGirder(nType);
	else					dValue = GetCamberByType_GenGirder(nType);

	return dValue;
}

double CCamber::GetCamberByType_GenGirder(long nType)
{
	switch (nType) 
	{
		case 0 : 
			return m_dCamberJongDan;
		case 1 :
			return m_dCamberPyungubae;
		case 2 :
			return m_dCamberSteelOnly;
		case 3 :
			return m_dCamberFixLoad;
		case 4 :
			return m_dCamberVividLoad;
		case 5 :
			return m_dCamberFactory;
		case 6 :
			return m_dCamberField;
		case 7 :
			return m_dCamberSteelRemove;
		case 8 :
			return m_dCamberVividRemove;
	}
	return 0;
}

double CCamber::GetCamberByType_TUGirder(long nType)
{
	switch (nType) 
	{
		case 0 :
			return m_dCamberJongDan;
		case 1 :
			return m_dCamberPyungubae;
		case 2 :
			return m_dCamberSteelOnly;
		case 3 :
			return m_dCamberConf;
		case 4 :
			return m_dCamberTurning;
		case 5 :
			return m_dCamberFixLoad;
		case 6 :
			return m_dCamberVividLoad;
		case 7 :
			return m_dCamberFactory;
		case 8 :
			return m_dCamberField;
		case 9 :
			return m_dCamberSteelRemove;
		case 10 :
			return m_dCamberVividRemove;
	}
	return 0;
}

void CCamber::SetOldStationCamber(double staJijum)
{
	if (m_nFlag < 2)
	{
		m_dStationCamber += staJijum;
	}
}
