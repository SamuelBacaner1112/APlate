// JointResult.cpp: implementation of the CJointResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJointResult::CJointResult()
{
	ClearData();
}

CJointResult::~CJointResult()
{

}

void CJointResult::Serialize(CArchive& ar)
{
//	long nFlag = 0;
	long nFlag = 1;	// TU연속

	if(ar.IsStoring())
	{
		ar << nFlag;
	}
	else
	{
		ar >> nFlag;
	}
	for(long n=0; n<8; n++)
	{
		m_vPreDeadSteel[n].Serialize(ar);		//합성전 사하중 - 강재자중
		if(nFlag>0)
		{
			m_vPreDeadConf2[n].Serialize(ar);			//합성전 사하중 - 구속콘크리트
			m_vPreDeadSteelConf[n].Serialize(ar);		//합성전 사하중 - 구속콘크리트
		}
		m_vPreDeadConf[n].Serialize(ar);		//합성전 사하중 - 구속콘크리트
		m_vPreDeadTurning[n].Serialize(ar);	//합성전 사하중 - 180 터닝
		m_vPreDeadSlab[n].Serialize(ar); //합성전 사하중 - 슬래브자중
		m_vPostDead[n].Serialize(ar);    //합성후 사하중
		m_vLiveMax[n].Serialize(ar);     //활하중 최대값
		m_vLiveMin[n].Serialize(ar);     //활하중 최소값
		m_vWindMax[n].Serialize(ar);     //풍하중 최대값
		m_vWindMin[n].Serialize(ar);     //풍하중 최소값
		m_vUnSettleMax[n].Serialize(ar); //지점침하 최소값
		m_vUnSettleMin[n].Serialize(ar); //지점침하 최대값
		m_vTotalMax[n].Serialize(ar);    //합계 최대값 
		m_vTotalMin[n].Serialize(ar);    //합계 최소값
	}
}

void CJointResult::ClearData()
{
	for(long n=0; n<8; n++)
	{
		m_vPreDeadSteel[n] = CVector(0,0,0);		//합성전 사하중 - 강재자중
		m_vPreDeadConf2[n] = CVector(0,0,0);		//합성전 사하중 - 구속콘크리트
		m_vPreDeadSteelConf[n] = CVector(0,0,0);	//합성전 사하중 - 강재자중
		m_vPreDeadConf[n] = CVector(0,0,0);		//합성전 사하중 - 구속콘크리트
		m_vPreDeadTurning[n] = CVector(0,0,0);	//합성전 사하중 - 180 터닝
		m_vPreDeadSlab[n] = CVector(0,0,0); //합성전 사하중 - 슬래브자중
		m_vPostDead[n] = CVector(0,0,0);    //합성후 사하중
		m_vLiveMax[n] = CVector(0,0,0);     //활하중 최대값
		m_vLiveMin[n] = CVector(0,0,0);     //활하중 최소값
		m_vWindMax[n] = CVector(0,0,0);     //풍하중 최대값
		m_vWindMin[n] = CVector(0,0,0);     //풍하중 최소값
		m_vUnSettleMax[n] = CVector(0,0,0); //지점침하 최소값
		m_vUnSettleMin[n] = CVector(0,0,0); //지점침하 최대값
		m_vTotalMax[n] = CVector(0,0,0);    //합계 최대값 
		m_vTotalMin[n] = CVector(0,0,0);    //합계 최소값
	}
}