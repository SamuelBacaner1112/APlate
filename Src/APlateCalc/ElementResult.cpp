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
			m_dPreDeadSteel[n][k] = 0;		//�ռ��� ������ - ��������
			m_dPreDeadConf2[n][k] = 0;		//�ռ��� ������ - ������ũ��Ʈ(���ӱ�)
			m_dPreDeadSteelConf[n][k] = 0;	//�ռ��� ������ - ����+������ũ��Ʈ(���ӱ�)
			m_dPreDeadConf[n][k] = 0;		//�ռ��� ������ - ������ũ��Ʈ
			m_dPreDeadTurning[n][k] = 0;	//�ռ��� ������ - 180 �ʹ�(�ܼ���)
			m_dPreDeadSlab[n][k] = 0;		//�ռ��� ������ - ���κ�+�ٴ���
			m_dPostDead[n][k] = 0;			//�ռ��� ������

			m_dLiveMax[n][k] = 0;			//Ȱ���� �ִ밪
			m_dLiveMin[n][k] = 0;			//Ȱ���� �ּҰ�
			m_dOnlyLiveMax[n][k] = 0;		//Ȱ���� (ǳ���� ����)�ִ밪
			m_dOnlyLiveMin[n][k] = 0;		//Ȱ���� (ǳ���� ����)�ּҰ�
			m_dUnSettleMax[n][k] = 0;		//����ħ�� �ּҰ�
			m_dUnSettleMin[n][k] = 0;		//����ħ�� �ִ밪

			m_dDBMax[n][k] = 0;       //DB �ִ밪(DB MAX)
			m_dDBMin[n][k] = 0;       //DB �ּҰ�(DB MIN)
			m_dDLMax[n][k] = 0;       //DL �ִ밪(DL MAX)
			m_dDLMin[n][k] = 0;       //DL �ּҰ�(DL MIN)
			m_dMTYMax[n][k] = 0;      //��������(Tank+Trailer) �ִ밪(MTY MAX)
			m_dMTYMin[n][k] = 0;      //��������(Tank+Trailer) �ּҰ�(MTY MIN)
			m_dPeopleMax[n][k] = 0;   //�������� �ִ밪(PEOPLE MAX)
			m_dPeopleMin[n][k] = 0;   //�������� �ּҰ�(PEOPLE MIN)
			m_dWindMax[n][k] = 0;     //ǳ���� �ִ밪(WIND MAX)
			m_dWindMin[n][k] = 0;     //ǳ���� �ּҰ�(WIND MIN)
			m_dTankMax[n][k] = 0;
			m_dTankMin[n][k] = 0;
			m_dTrailerMax[n][k] = 0;
			m_dTrailerMin[n][k] = 0;
			
			m_dTotalMax[n][k] = 0;    //�հ� �ִ밪 
			m_dTotalMin[n][k] = 0;    //�հ� �ּҰ�
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
