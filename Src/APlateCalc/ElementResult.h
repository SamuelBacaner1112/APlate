// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ElementResult.h: interface for the CElementResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTRESULT_H__EB685831_81F6_43A5_B63E_9FB6D07BAADE__INCLUDED_)
#define AFX_ELEMENTRESULT_H__EB685831_81F6_43A5_B63E_9FB6D07BAADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CElementResult : public CObject
{
public:
	CElementResult();
	virtual ~CElementResult();

	void Serialize(CArchive& ar);
	void ClearData();

	///< [하중종류][분할요소의 인덱스]
	double m_dPreDeadSteel[8][10];		//합성전 사하중 - 강재자중
	double m_dPreDeadConf2[8][10];		//합성전 사하중 - 구속콘크리트(연속교)
	double m_dPreDeadSteelConf[8][10];	//합성전 사하중 - 강재+구속콘크리트(연속교)
	double m_dPreDeadConf[8][10];		//합성전 사하중 - 구속콘크리트
	double m_dPreDeadTurning[8][10];	//합성전 사하중 - 180 터닝(단순교)
	double m_dPreDeadSlab[8][10];		//합성전 사하중 - 가로보+바닥판
	double m_dPostDead[8][10];			//합성후 사하중

	double m_dLiveMax[8][10];     //활하중 최대값
	double m_dLiveMin[8][10];     //활하중 최소값
	double m_dOnlyLiveMax[8][10]; //활하중 (풍하중 제외)최대값
	double m_dOnlyLiveMin[8][10]; //활하중 (풍하중 제외)최소값
	double m_dUnSettleMax[8][10]; //지점침하 최소값
	double m_dUnSettleMin[8][10]; //지점침하 최대값

	double m_dDBMax[8][10];       //DB 최대값(DB MAX)
	double m_dDBMin[8][10];       //DB 최소값(DB MIN)
	double m_dDLMax[8][10];       //DL 최대값(DL MAX)
	double m_dDLMin[8][10];       //DL 최소값(DL MIN)
	double m_dMTYMax[8][10];      //군사하중(Tank+Trailer) 최대값(MTY MAX)
	double m_dMTYMin[8][10];      //군사하중(Tank+Trailer) 최소값(MTY MIN)
	double m_dPeopleMax[8][10];   //군중하중 최대값(PEOPLE MAX)
	double m_dPeopleMin[8][10];   //군중하중 최소값(PEOPLE MIN)
	double m_dWindMax[8][10];     //풍하중 최대값(WIND MAX)
	double m_dWindMin[8][10];     //풍하중 최소값(WIND MIN)
	double m_dTankMax[8][10];
	double m_dTankMin[8][10];
	double m_dTrailerMax[8][10];
	double m_dTrailerMin[8][10];
	
	double m_dTotalMax[8][10];    //합계 최대값 
	double m_dTotalMin[8][10];    //합계 최소값
};

#endif // !defined(AFX_ELEMENTRESULT_H__EB685831_81F6_43A5_B63E_9FB6D07BAADE__INCLUDED_)
