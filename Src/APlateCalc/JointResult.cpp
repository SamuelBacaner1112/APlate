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
	long nFlag = 1;	// TU����

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
		m_vPreDeadSteel[n].Serialize(ar);		//�ռ��� ������ - ��������
		if(nFlag>0)
		{
			m_vPreDeadConf2[n].Serialize(ar);			//�ռ��� ������ - ������ũ��Ʈ
			m_vPreDeadSteelConf[n].Serialize(ar);		//�ռ��� ������ - ������ũ��Ʈ
		}
		m_vPreDeadConf[n].Serialize(ar);		//�ռ��� ������ - ������ũ��Ʈ
		m_vPreDeadTurning[n].Serialize(ar);	//�ռ��� ������ - 180 �ʹ�
		m_vPreDeadSlab[n].Serialize(ar); //�ռ��� ������ - ����������
		m_vPostDead[n].Serialize(ar);    //�ռ��� ������
		m_vLiveMax[n].Serialize(ar);     //Ȱ���� �ִ밪
		m_vLiveMin[n].Serialize(ar);     //Ȱ���� �ּҰ�
		m_vWindMax[n].Serialize(ar);     //ǳ���� �ִ밪
		m_vWindMin[n].Serialize(ar);     //ǳ���� �ּҰ�
		m_vUnSettleMax[n].Serialize(ar); //����ħ�� �ּҰ�
		m_vUnSettleMin[n].Serialize(ar); //����ħ�� �ִ밪
		m_vTotalMax[n].Serialize(ar);    //�հ� �ִ밪 
		m_vTotalMin[n].Serialize(ar);    //�հ� �ּҰ�
	}
}

void CJointResult::ClearData()
{
	for(long n=0; n<8; n++)
	{
		m_vPreDeadSteel[n] = CVector(0,0,0);		//�ռ��� ������ - ��������
		m_vPreDeadConf2[n] = CVector(0,0,0);		//�ռ��� ������ - ������ũ��Ʈ
		m_vPreDeadSteelConf[n] = CVector(0,0,0);	//�ռ��� ������ - ��������
		m_vPreDeadConf[n] = CVector(0,0,0);		//�ռ��� ������ - ������ũ��Ʈ
		m_vPreDeadTurning[n] = CVector(0,0,0);	//�ռ��� ������ - 180 �ʹ�
		m_vPreDeadSlab[n] = CVector(0,0,0); //�ռ��� ������ - ����������
		m_vPostDead[n] = CVector(0,0,0);    //�ռ��� ������
		m_vLiveMax[n] = CVector(0,0,0);     //Ȱ���� �ִ밪
		m_vLiveMin[n] = CVector(0,0,0);     //Ȱ���� �ּҰ�
		m_vWindMax[n] = CVector(0,0,0);     //ǳ���� �ִ밪
		m_vWindMin[n] = CVector(0,0,0);     //ǳ���� �ּҰ�
		m_vUnSettleMax[n] = CVector(0,0,0); //����ħ�� �ּҰ�
		m_vUnSettleMin[n] = CVector(0,0,0); //����ħ�� �ִ밪
		m_vTotalMax[n] = CVector(0,0,0);    //�հ� �ִ밪 
		m_vTotalMin[n] = CVector(0,0,0);    //�հ� �ּҰ�
	}
}