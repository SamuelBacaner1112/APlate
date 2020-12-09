// RebarData.cpp: implementation of the CRebarData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RebarData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRebarData::CRebarData()
{
	//ö�ٹ���(����)	
	m_dLengthD1    = 0;//��ι��ö��
	m_dLengthD2[0] = 0;//�Ϻι��ö��
	m_dLengthD2[1] = 0;//�ܺκ������ö��(���)-[����][����]
	m_dLengthD3    = 0;//�ܺκ������ö��(���)-[����][����]
	m_dLengthD4[0] = 0;//�ܺκ������ö��(�Ϻ�)-[����][����]
	m_dLengthD4[1] = 0;//�ܺκ������ö��(�Ϻ�)-[����][����]
	m_dLengthD5    = 0;
	m_dLengthD6    = 0;
	m_dLengthDW[0] = 0;//��ȣ�����ö��-[����][����]
	m_dLengthDW[1] = 0;//��ȣ�����ö��-[����][����]
	
	m_dLength1    = 0;//�Ϲݺ���ö��(���)
	m_dLength2    = 0;//�Ϲݺ���ö��(�Ϻ�)
	m_dLength3[0] = 0;//ĵƿ������ ���� ��ö��
	m_dLength3[1] = 0;//ĵƿ������ ���� ��ö��
	m_dLength4    = 0;//�ܺ���ö��(���)
	m_dLength5    = 0;//�ܺ���ö��(�Ϻ�)
	m_dLengthW[0] = 0;//��ȣ����ö��-[����][����]
	m_dLengthW[1] = 0;//��ȣ����ö��-[����][����]
	m_dLengthG    = 0; //���͵� ����ö��
	m_dLengthS1   = 0;//������1
	m_dLengthS2   = 0;//������2
	m_dHeightS1   = 0;//������1
	m_dHeightS2   = 0;//������2
	//ö�ٹ���(����)
	m_nRebarD1    = 0;//�Ϲݺ���ö��(���)
	m_nRebarD2[0] = 0;//�Ϲݺ���ö��(�Ϻ�)
	m_nRebarD2[1] = 0;//�ܺ���ö��(���)
	m_nRebarD3    = 0;//�ܺ���ö��(���)
	m_nRebarD4[0] = 0;//�ܺ���ö��(�Ϻ�)
	m_nRebarD4[1] = 0;//�ܺ���ö��(�Ϻ�)
	m_nRebarD5	  = 0;//�ܺ���ö��(�Ϻ�)
	m_nRebarD6	  = 0;//�ܺ���ö��(�Ϻ�)
	m_nRebarDW[0] = 0;//��ȣ�����ö��-[����][����]
	m_nRebarDW[1] = 0;//��ȣ�����ö��-[����][����]

	m_nRebar1    = 0;//�Ϲݺ���ö��(���)
	m_nRebar2    = 0;//�Ϲݺ���ö��(�Ϻ�)
	m_nRebar3[0] = 0;//ĵƿ������ ���� ��ö��
	m_nRebar3[1] = 0;//ĵƿ������ ���� ��ö��
	m_nRebar4    = 0;//�ܺ���ö��(���)
	m_nRebar5    = 0;//�ܺ���ö��(�Ϻ�)
	m_nRebarW[0] = 0;//��ȣ����ö��-[����][����]
	m_nRebarW[1] = 0;//��ȣ����ö��-[����][����]
	m_nRebarG    = 0; //���͵� ����ö��
	m_nRebarS1H  = 0;//������1
	m_nRebarS2H  = 0;//������2
	m_nRebarS1J  = 0;//������1
	m_nRebarS2J  = 0;//������2
	//ö�ٻ�����
	m_nSpacersuPerM2 = 4;
	m_nStrupDia      = 4;
	m_dDetailL1[0]   = 0;
	m_dDetailL1[1]   = 0;
	m_dDetailL2[0]   = 0;
	m_dDetailL2[1]   = 0;
	m_dDetailL3[0]   = 0;
	m_dDetailL3[1]   = 0;
	m_dDetailL4[0]   = 0;
	m_dDetailL4[1]   = 0;

	m_dLengthShearEnd = 900;
	m_dLengthGuardCTC = 200;
	m_nSpacerDia      = 2;
	m_dRadLengthStrup = 100;
	m_dDiaSpacer      = 0;     //����������
	m_dDiaStrupBogang = 0;//��Ʈ������ö������
	m_nPier           = 0;
}

CRebarData::~CRebarData()
{

}
void CRebarData::Serialize(CArchive& ar)
{	
	long nFlag = 0;
	m_arrSlabSideUp.Serialize(ar);//������ ���� �������Ʈ(�Ŵ� ��.����, ���ߺй谡����ġ)
	m_arrSlabSideDn.Serialize(ar);//������ ���� �Ϻ�����Ʈ(����Ʈ���� ���� ���ϴ� ����Ʈ)
	m_arrSlabFrontCenUp.Serialize(ar);//������ ���� �������Ʈ
	m_arrSlabFrontCenDn.Serialize(ar);//������ ���� �Ϻ�����Ʈ
	m_arrSlabFrontDanUp.Serialize(ar);//������ �ĸ� �������Ʈ
	m_arrSlabFrontDanDn.Serialize(ar);//������ �ĸ� �Ϻ�����Ʈ
	m_arrSlabPlan.Serialize(ar);//������ ��� ����Ʈ

	m_arrRebarSideUp.Serialize(ar);//������ ���� �������Ʈ(�Ŵ� ��.����, ���ߺй谡����ġ)
	m_arrRebarSideDn.Serialize(ar);//������ ���� �Ϻ�����Ʈ(����Ʈ���� ���� ���ϴ� ����Ʈ)
	m_arrRebarFrontCenUp.Serialize(ar);//������ ���� �������Ʈ
	m_arrRebarFrontCenDn.Serialize(ar);//������ ���� �Ϻ�����Ʈ
	m_arrRebarFrontDanUp.Serialize(ar);//������ �ĸ� �������Ʈ
	m_arrRebarFrontDanDn.Serialize(ar);//������ �ĸ� �Ϻ�����Ʈ

	if(ar.IsStoring()) 
	{
		//ö�ٹ���(����)	
		ar << m_dLengthD1;
		ar << m_dLengthD2[0];
		ar << m_dLengthD2[1];
		ar << m_dLengthD3;
		ar << m_dLengthD4[0];
		ar << m_dLengthD4[1];
		ar << m_dLengthD5;
		ar << m_dLengthD6;
		ar << m_dLengthDW[0];
		ar << m_dLengthDW[1];
		
		ar << m_dLength1;
		ar << m_dLength2;
		ar << m_dLength3[0];
		ar << m_dLength3[1];
		ar << m_dLength4;   
		ar << m_dLength5;   
		ar << m_dLengthW[0];
		ar << m_dLengthW[1];
		ar << m_dLengthG; 
		ar << m_dLengthS1;
		ar << m_dLengthS2;
		ar << m_dHeightS1;
		ar << m_dHeightS2;
		//ö�ٹ���(����)
		ar << m_nRebarD1;
		ar << m_nRebarD2[0];
		ar << m_nRebarD2[1];
		ar << m_nRebarD3;
		ar << m_nRebarD4[0];
		ar << m_nRebarD4[1];
		ar << m_nRebarD5;
		ar << m_nRebarD6;
		ar << m_nRebarDW[0];
		ar << m_nRebarDW[1];

		ar << m_nRebar1;
		ar << m_nRebar2;
		ar << m_nRebar3[0];
		ar << m_nRebar3[1];
		ar << m_nRebar4;
		ar << m_nRebar5;
		ar << m_nRebarW[0];
		ar << m_nRebarW[1];
		ar << m_nRebarG; 
		ar << m_nRebarS1H;
		ar << m_nRebarS2H;
		ar << m_nRebarS1J;
		ar << m_nRebarS2J;
		ar << m_nSpacersuPerM2;
		ar << m_nStrupDia;
		ar << m_dLengthShearEnd;
		ar << m_dLengthGuardCTC;
		ar << m_nSpacerDia;
		ar << m_dRadLengthStrup;
		ar << m_dDiaSpacer;     //����������
		ar << m_dDiaStrupBogang;//��Ʈ������ö������
		ar << m_dDetailL1[0];
		ar << m_dDetailL1[1];
		ar << m_dDetailL2[0];
		ar << m_dDetailL2[1];
		ar << m_dDetailL3[0];
		ar << m_dDetailL3[1];
		ar << m_dDetailL4[0];
		ar << m_dDetailL4[1];
		ar << m_nPier;
	}
	else
	{
		//ö�ٹ���(����)	
		ar >> m_dLengthD1;
		ar >> m_dLengthD2[0];
		ar >> m_dLengthD2[1];
		ar >> m_dLengthD3;
		ar >> m_dLengthD4[0];
		ar >> m_dLengthD4[1];
		ar >> m_dLengthD5;
		ar >> m_dLengthD6;
		ar >> m_dLengthDW[0];
		ar >> m_dLengthDW[1];
		
		ar >> m_dLength1;
		ar >> m_dLength2;
		ar >> m_dLength3[0];
		ar >> m_dLength3[1];
		ar >> m_dLength4;   
		ar >> m_dLength5;   
		ar >> m_dLengthW[0];
		ar >> m_dLengthW[1];
		ar >> m_dLengthG; 
		ar >> m_dLengthS1;
		ar >> m_dLengthS2;
		ar >> m_dHeightS1;
		ar >> m_dHeightS2;
		//ö�ٹ���(����)
		ar >> m_nRebarD1;
		ar >> m_nRebarD2[0];
		ar >> m_nRebarD2[1];
		ar >> m_nRebarD3;
		ar >> m_nRebarD4[0];
		ar >> m_nRebarD4[1];
		ar >> m_nRebarD5;
		ar >> m_nRebarD6;
		ar >> m_nRebarDW[0];
		ar >> m_nRebarDW[1];

		ar >> m_nRebar1;
		ar >> m_nRebar2;
		ar >> m_nRebar3[0];
		ar >> m_nRebar3[1];
		ar >> m_nRebar4;
		ar >> m_nRebar5;
		ar >> m_nRebarW[0];
		ar >> m_nRebarW[1];
		ar >> m_nRebarG; 
		ar >> m_nRebarS1H;
		ar >> m_nRebarS2H;
		ar >> m_nRebarS1J;
		ar >> m_nRebarS2J;
		ar >> m_nSpacersuPerM2;
		ar >> m_nStrupDia;
		ar >> m_dLengthShearEnd;
		ar >> m_dLengthGuardCTC;
		ar >> m_nSpacerDia;
		ar >> m_dRadLengthStrup;
		ar >> m_dDiaSpacer;     //����������
		ar >> m_dDiaStrupBogang;//��Ʈ������ö������
		ar >> m_dDetailL1[0];
		ar >> m_dDetailL1[1];
		ar >> m_dDetailL2[0];
		ar >> m_dDetailL2[1];
		ar >> m_dDetailL3[0];
		ar >> m_dDetailL3[1];
		ar >> m_dDetailL4[0];
		ar >> m_dDetailL4[1];
		ar >> m_nPier;
	}
}
