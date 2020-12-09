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

	///< [��������][���ҿ���� �ε���]
	double m_dPreDeadSteel[8][10];		//�ռ��� ������ - ��������
	double m_dPreDeadConf2[8][10];		//�ռ��� ������ - ������ũ��Ʈ(���ӱ�)
	double m_dPreDeadSteelConf[8][10];	//�ռ��� ������ - ����+������ũ��Ʈ(���ӱ�)
	double m_dPreDeadConf[8][10];		//�ռ��� ������ - ������ũ��Ʈ
	double m_dPreDeadTurning[8][10];	//�ռ��� ������ - 180 �ʹ�(�ܼ���)
	double m_dPreDeadSlab[8][10];		//�ռ��� ������ - ���κ�+�ٴ���
	double m_dPostDead[8][10];			//�ռ��� ������

	double m_dLiveMax[8][10];     //Ȱ���� �ִ밪
	double m_dLiveMin[8][10];     //Ȱ���� �ּҰ�
	double m_dOnlyLiveMax[8][10]; //Ȱ���� (ǳ���� ����)�ִ밪
	double m_dOnlyLiveMin[8][10]; //Ȱ���� (ǳ���� ����)�ּҰ�
	double m_dUnSettleMax[8][10]; //����ħ�� �ּҰ�
	double m_dUnSettleMin[8][10]; //����ħ�� �ִ밪

	double m_dDBMax[8][10];       //DB �ִ밪(DB MAX)
	double m_dDBMin[8][10];       //DB �ּҰ�(DB MIN)
	double m_dDLMax[8][10];       //DL �ִ밪(DL MAX)
	double m_dDLMin[8][10];       //DL �ּҰ�(DL MIN)
	double m_dMTYMax[8][10];      //��������(Tank+Trailer) �ִ밪(MTY MAX)
	double m_dMTYMin[8][10];      //��������(Tank+Trailer) �ּҰ�(MTY MIN)
	double m_dPeopleMax[8][10];   //�������� �ִ밪(PEOPLE MAX)
	double m_dPeopleMin[8][10];   //�������� �ּҰ�(PEOPLE MIN)
	double m_dWindMax[8][10];     //ǳ���� �ִ밪(WIND MAX)
	double m_dWindMin[8][10];     //ǳ���� �ּҰ�(WIND MIN)
	double m_dTankMax[8][10];
	double m_dTankMin[8][10];
	double m_dTrailerMax[8][10];
	double m_dTrailerMin[8][10];
	
	double m_dTotalMax[8][10];    //�հ� �ִ밪 
	double m_dTotalMin[8][10];    //�հ� �ּҰ�
};

#endif // !defined(AFX_ELEMENTRESULT_H__EB685831_81F6_43A5_B63E_9FB6D07BAADE__INCLUDED_)
