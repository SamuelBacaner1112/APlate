// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// JointResult.h: interface for the CJointResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINTRESULT_H__B1ED97CF_01A6_49B9_A3D9_7BC7D9FB9DC5__INCLUDED_)
#define AFX_JOINTRESULT_H__B1ED97CF_01A6_49B9_A3D9_7BC7D9FB9DC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CJointResult  : public CObject
{
public:
	CJointResult();
	virtual ~CJointResult();

	void Serialize(CArchive& ar);
	void ClearData();
	
	//[하중종류]
	CVector m_vPreDeadSteel[8];			//합성전 사하중 - 강재자중
	CVector m_vPreDeadConf2[8];			//합성전 사하중 - 구속콘크리트(연속교)
	CVector m_vPreDeadSteelConf[8];		//합성전 사하중 - 강재+구속콘크리트(연속교)
	CVector m_vPreDeadConf[8];			//합성전 사하중 - 구속콘크리트
	CVector m_vPreDeadTurning[8];		//합성전 사하중 - 180 터닝(단순교)
	CVector m_vPreDeadSlab[8];			//합성전 사하중 - 슬래브자중
	CVector m_vPostDead[8];				//합성후 사하중
	CVector m_vLiveMax[8];				//활하중 최대값
	CVector m_vLiveMin[8];				//활하중 최소값
	CVector m_vWindMax[8];				//풍하중 최대값
	CVector m_vWindMin[8];				//풍하중 최소값
	CVector m_vUnSettleMax[8];			//지점침하 최소값
	CVector m_vUnSettleMin[8];			//지점침하 최대값
	CVector m_vTotalMax[8];				//합계 최대값 
	CVector m_vTotalMin[8];				//합계 최소값

};

#endif // !defined(AFX_JOINTRESULT_H__B1ED97CF_01A6_49B9_A3D9_7BC7D9FB9DC5__INCLUDED_)
