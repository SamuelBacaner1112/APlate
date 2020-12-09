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
	
	//[��������]
	CVector m_vPreDeadSteel[8];			//�ռ��� ������ - ��������
	CVector m_vPreDeadConf2[8];			//�ռ��� ������ - ������ũ��Ʈ(���ӱ�)
	CVector m_vPreDeadSteelConf[8];		//�ռ��� ������ - ����+������ũ��Ʈ(���ӱ�)
	CVector m_vPreDeadConf[8];			//�ռ��� ������ - ������ũ��Ʈ
	CVector m_vPreDeadTurning[8];		//�ռ��� ������ - 180 �ʹ�(�ܼ���)
	CVector m_vPreDeadSlab[8];			//�ռ��� ������ - ����������
	CVector m_vPostDead[8];				//�ռ��� ������
	CVector m_vLiveMax[8];				//Ȱ���� �ִ밪
	CVector m_vLiveMin[8];				//Ȱ���� �ּҰ�
	CVector m_vWindMax[8];				//ǳ���� �ִ밪
	CVector m_vWindMin[8];				//ǳ���� �ּҰ�
	CVector m_vUnSettleMax[8];			//����ħ�� �ּҰ�
	CVector m_vUnSettleMin[8];			//����ħ�� �ִ밪
	CVector m_vTotalMax[8];				//�հ� �ִ밪 
	CVector m_vTotalMin[8];				//�հ� �ּҰ�

};

#endif // !defined(AFX_JOINTRESULT_H__B1ED97CF_01A6_49B9_A3D9_7BC7D9FB9DC5__INCLUDED_)
