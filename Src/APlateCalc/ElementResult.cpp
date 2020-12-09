// ElementResult.cpp: implementation of the CElementResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElementResult::CElementResult()
{
	ClearData();
}

CElementResult::~CElementResult()
{

}

void CElementResult::ClearData()
{
	for(long n=0; n<8; n++)
	{
		for(long k=0; k<10; k++)
		{
			m_dPreDeadSteel[n][k] = 0;		//합성전 사하중 - 강재자중
			m_dPreDeadConf2[n][k] = 0;		//합성전 사하중 - 구속콘크리트(연속교)
			m_dPreDeadSteelConf[n][k] = 0;	//합성전 사하중 - 강재+구속콘크리트(연속교)
			m_dPreDeadConf[n][k] = 0;		//합성전 사하중 - 구속콘크리트
			m_dPreDeadTurning[n][k] = 0;	//합성전 사하중 - 180 터닝(단순교)
			m_dPreDeadSlab[n][k] = 0;		//합성전 사하중 - 가로보+바닥판
			m_dPostDead[n][k] = 0;			//합성후 사하중

			m_dLiveMax[n][k] = 0;			//활하중 최대값
			m_dLiveMin[n][k] = 0;			//활하중 최소값
			m_dOnlyLiveMax[n][k] = 0;		//활하중 (풍하중 제외)최대값
			m_dOnlyLiveMin[n][k] = 0;		//활하중 (풍하중 제외)최소값
			m_dUnSettleMax[n][k] = 0;		//지점침하 최소값
			m_dUnSettleMin[n][k] = 0;		//지점침하 최대값

			m_dDBMax[n][k] = 0;       //DB 최대값(DB MAX)
			m_dDBMin[n][k] = 0;       //DB 최소값(DB MIN)
			m_dDLMax[n][k] = 0;       //DL 최대값(DL MAX)
			m_dDLMin[n][k] = 0;       //DL 최소값(DL MIN)
			m_dMTYMax[n][k] = 0;      //군사하중(Tank+Trailer) 최대값(MTY MAX)
			m_dMTYMin[n][k] = 0;      //군사하중(Tank+Trailer) 최소값(MTY MIN)
			m_dPeopleMax[n][k] = 0;   //군중하중 최대값(PEOPLE MAX)
			m_dPeopleMin[n][k] = 0;   //군중하중 최소값(PEOPLE MIN)
			m_dWindMax[n][k] = 0;     //풍하중 최대값(WIND MAX)
			m_dWindMin[n][k] = 0;     //풍하중 최소값(WIND MIN)
			m_dTankMax[n][k] = 0;
			m_dTankMin[n][k] = 0;
			m_dTrailerMax[n][k] = 0;
			m_dTrailerMin[n][k] = 0;
			
			m_dTotalMax[n][k] = 0;    //합계 최대값 
			m_dTotalMin[n][k] = 0;    //합계 최소값
		}
	}
}

void CElementResult::Serialize(CArchive& ar)
{
	long	nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
	}
	else
	{
		ar >> nFlag;
	}
}
