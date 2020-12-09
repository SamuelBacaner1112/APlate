// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RebarData.h: interface for the CRebarData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REBARDATA_H__511FA521_97A4_4312_A6B1_F463ED70B0DE__INCLUDED_)
#define AFX_REBARDATA_H__511FA521_97A4_4312_A6B1_F463ED70B0DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBasicIndex;
class CPlateGirderApp;
class CPlateBridgeApp;

class AFX_EXT_CLASS CRebarData
{
public:
	CRebarData();
	virtual ~CRebarData();

	void Serialize(CArchive& ar);
	//�������� ���̽�
	CDPointArray m_arrSlabSideUp;//������ ���� �������Ʈ(�Ŵ� ��.����, ���ߺй谡����ġ)
	CDPointArray m_arrSlabSideDn;//������ ���� �Ϻ�����Ʈ(����Ʈ���� ���� ���ϴ� ����Ʈ)
	CDPointArray m_arrSlabFrontCenUp;//������ ���� �������Ʈ
	CDPointArray m_arrSlabFrontCenDn;//������ ���� �Ϻ�����Ʈ
	CDPointArray m_arrSlabFrontDanUp;//������ �ĸ� �������Ʈ
	CDPointArray m_arrSlabFrontDanDn;//������ �ĸ� �Ϻ�����Ʈ
	CDPointArray m_arrSlabPlan;//������ ��� ����Ʈ
	//ö�� ���� �� ����
	CDPointArray m_arrRebarSideUp;//������ ���� �������Ʈ(�Ŵ� ��.����, ���ߺй谡����ġ)
	CDPointArray m_arrRebarSideDn;//������ ���� �Ϻ�����Ʈ(����Ʈ���� ���� ���ϴ� ����Ʈ)
	CDPointArray m_arrRebarFrontCenUp;//������ ���� �������Ʈ
	CDPointArray m_arrRebarFrontCenDn;//������ ���� �Ϻ�����Ʈ
	CDPointArray m_arrRebarFrontDanUp;//������ �ĸ� �������Ʈ
	CDPointArray m_arrRebarFrontDanDn;//������ �ĸ� �Ϻ�����Ʈ
	//ö�ٹ���(����)	
	double m_dLengthD1;//�Ϲݺ� ��ι��ö��
	double m_dLengthD2[2];//�ܺκ������ö��(���)-[����][����]
	double m_dLengthD3;//�Ϲݺ� �Ϻι��ö��
	double m_dLengthD4[2];//�ܺκ������ö��(�Ϻ�)-[����][����]
	double m_dLengthD5;//�����κ������ö��(���)
	double m_dLengthD6;//�����κ������ö��(�Ϻ�)
	double m_dLengthDW[2];//��ȣ�����ö��-[����][����]
	
	double m_dLength1;//�Ϲݺ���ö��(���)
	double m_dLength2;//�Ϲݺ���ö��(�Ϻ�)
	double m_dLength3[2];//ĵƿ������ ���� ��ö��
	double m_dLength4;//�ܺ���ö��(���)
	double m_dLength5;//�ܺ���ö��(�Ϻ�)
	double m_dLengthW[2];//��ȣ����ö��-[����][����]
	double m_dLengthG; //���͵� ����ö��
	double m_dLengthS1;//������1
	double m_dLengthS2;//������2
	double m_dHeightS1;//������1
	double m_dHeightS2;//������2

	//ö�ٹ���(����)
	long   m_nRebarD1;//�Ϲݺ� ��ι��ö��
	long   m_nRebarD2[2];//�ܺκ������ö��(���)-[����][����]
	long   m_nRebarD3;//�Ϲݺ� �Ϻι��ö��
	long   m_nRebarD4[2];//�ܺκ������ö��(�Ϻ�)-[����][����]
	long   m_nRebarD5;//�����κ������ö��(���)
	long   m_nRebarD6;//�����κ������ö��(�Ϻ�)
	double m_nRebarDW[2];//��ȣ�����ö��-[����][����]

	long   m_nRebar1;//�Ϲݺ���ö��(���)
	long   m_nRebar2;//�Ϲݺ���ö��(�Ϻ�)
	long   m_nRebar3[2];//ĵƿ������ ���� ��ö��
	long   m_nRebar4;//�ܺ���ö��(���)
	long   m_nRebar5;//�ܺ���ö��(�Ϻ�)
	long   m_nRebarW[2];//��ȣ����ö��-[����][����]
	double m_nRebarG; //���͵� ����ö��
	double m_nRebarS1H;//Ⱦ���� ������1
	double m_nRebarS2H;//Ⱦ���� ������2
	double m_nRebarS1J;//������ ������1
	double m_nRebarS2J;//������ ������2
	//ö�ٻ�����
	long   m_nSpacersuPerM2;//����������(m2)�����簹��
	long   m_nStrupDia;//����ö�ٺ���������
	double m_nSpacerDia;//����������
	double m_dLengthShearEnd;//���ܺ���ö�� �糡���� 

	double m_dDetailL1[2];
	double m_dDetailL2[2];
	double m_dDetailL3[2];
	double m_dDetailL4[2];

	double m_dLengthGuardCTC;//��ȣ��CTC
	double m_dRadLengthStrup;//���ܺ���ö���غκ��� �ݰ�
	double m_dDiaSpacer;     //����������
	double m_dDiaStrupBogang;//��Ʈ������ö������
	long   m_nPier;          //�����κθ��Ʈ����ö�ٻ��� �Ǿ���ġ���� 

};

#endif // !defined(AFX_REBARDATA_H__511FA521_97A4_4312_A6B1_F463ED70B0DE__INCLUDED_)
