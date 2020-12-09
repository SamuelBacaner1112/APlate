// SteelSection.cpp: implementation of the CSteelSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelSection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DATASU	5

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelSection::CSteelSection()
{
	m_bSameBuje = TRUE;
	ClearKey();
	DataInit();
}

CSteelSection::~CSteelSection()
{
	RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
//	class	:	CSteelSection
void CSteelSection::RemoveAll()
{
	long size = m_SectionArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_SectionArr[n];
	m_SectionArr.RemoveAll();
}

void CSteelSection::Serialize(CArchive& ar)
{
	long size	= 0;
//	long nFlag	= 0;
//	long nFlag	= 1;	// FillerPlate �߰� ..090521 lina
//	long nFlag	= 2;
	long nFlag	= 3;	// ���κ� HSteel,�������ܿ����� �������� ����

	if(ar.IsStoring()) 
	{
		ar << nFlag;
		size = 	m_SectionArr.GetSize();
		ar << size;
		for(long n = 0 ; n < size ; n++)
			m_SectionArr[n]->Serialize(ar);
		ar << m_bSameBuje;
	}
	else 
	{
		ar >> nFlag;
		RemoveAll();
		ar >> size;

		for(long n = 0 ; n < size ; n++)
		{
			CSteelSectionData *pData = new CSteelSectionData;
			pData->Serialize(ar);

			if(nFlag<3 && (n==CP_STEEL || n==CG_STEEL) && pData->m_strName!="H����")
			{
				CSteelSectionData *pData2 = new CSteelSectionData;
				pData2->m_strType		= pData->m_strType;
				pData2->m_strName		= "H����";
				pData2->m_strShape		= "����";
				pData2->m_strClass		= "�κ���";
				pData2->m_strSymbol		= pData->m_strSymbol;
				pData2->m_strSizeType	= "��������";
				m_SectionArr.InsertAt(n, pData2);
			}
			if(nFlag<3 && n==GI_SQ && pData->m_strName.Find("����")==-1)
			{
				CSteelSectionData *pData2 = new CSteelSectionData;
				pData2->m_strType		= pData->m_strType;
				pData2->m_strName		= "���ܿ�����(����)";
				pData2->m_strShape		= "�÷���Ʈ";
				pData2->m_strClass		= "�κ���";
				pData2->m_strSymbol		= GetStringData(G_F_U, BDISE_SYMBOL);
				pData2->m_strSizeType	= "��������";
				m_SectionArr.InsertAt(n, pData2);
			}
			m_SectionArr.Add(pData);
			if(nFlag<1 && pData->m_strType=="��� ��������"		&& pData->m_strName=="�ϸ� ������") SerializeFlag1(ar, pData);
			if(nFlag<1 && pData->m_strType=="�Ϻ� ��������"		&& pData->m_strName=="��� ������") SerializeFlag1(ar, pData);
			if(nFlag<1 && pData->m_strType=="������ ��������"	&& pData->m_strName=="���� ������") SerializeFlag1(ar, pData);
		}
		if(nFlag>1)
			ar >> m_bSameBuje;
	}
}

// nFlag = 1 �Ǹ鼭 �߰��� ����
void CSteelSection::SerializeFlag1(CArchive& ar, CSteelSectionData *pData)
{
	CSteelSectionData *pDataAdd = new CSteelSectionData;
	pDataAdd->m_strType		= pData->m_strType;
	pDataAdd->m_strName		= "ä����";
	pDataAdd->m_strShape	= "�÷���Ʈ";
	pDataAdd->m_strClass	= pData->m_strClass;
	pDataAdd->m_strSymbol	= pData->m_strSymbol;
	pDataAdd->m_strSizeType	= pData->m_strSizeType;

	m_SectionArr.Add(pDataAdd);	
}

void CSteelSection::DataInit()
{
	char* szBaseStr[] = {
	"�Ϲ� ,����,�÷���Ʈ,�ֺ���,SM490B,��������",					// G_F_U 0	
	"�Ϲ� ,����,�÷���Ʈ,�ֺ���,SM490B,��������",
	"�Ϲ� ,������,�÷���Ʈ,�ֺ���,SM490B,��������",
	"�Ϲ� ,���򺸰���(��),�÷���Ʈ,�ֺ���,SM490B,��������",
	"�Ϲ� ,���򺸰���(��),�÷���Ʈ,�ֺ���,SM490B,��������",
	
	"�Ϲ� ,����������,�÷���Ʈ,�κ���,SM400B,��������",
	"�Ϲ� ,����������,�÷���Ʈ,�ֺ���,SM490B,��������",

	"�����극�̽�, ������, ����, �κ���, SM400B,��������",
	"�����극�̽�, ������, ����, �κ���, SM400B,��������",
	"�����극�̽�, ����, ����, �κ���, SM400B,��������",
	"�����극�̽�, ������, �÷���Ʈ, �κ���, SM400B,��������",
	"�����극�̽�, ��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",

	"����극�̽�, ����, ����, �κ���, SM400B,��������",
	"����극�̽�, ������, �÷���Ʈ, �κ���, SM400B,��������",
	"����극�̽�, ��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",				// VB_BOLT

	"��� ��������,�ܺ� �����,�÷���Ʈ,�ֺ���,SM490B,��������",
	"��� ��������,�ϸ� ������,�÷���Ʈ,�ֺ���,SM490B,��������",
	"��� ��������,ä����,�÷���Ʈ,�ֺ���,SM490B,��������",	// 090521 �߰� nFlag=1
	"��� ��������,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",
	"�Ϻ� ��������,���� ������,�÷���Ʈ,�ֺ���,SM490B,��������",
	"�Ϻ� ��������,��� ������,�÷���Ʈ,�ֺ���,SM490B,��������",
	"�Ϻ� ��������,ä����,�÷���Ʈ,�ֺ���,SM490B,��������",	// 090521 �߰� nFlag=1
	"�Ϻ� ��������,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",
	"������ ��������,���Ʈ ������,�÷���Ʈ,�ֺ���,SM490B,��������",
	"������ ��������,���� ������,�÷���Ʈ,�ֺ���,SM490B,��������",
	"������ ��������,ä����,�÷���Ʈ,�ֺ���,SM490B,��������",	// 090521 �߰� nFlag=1
	"������ ��������,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",

	"���κ� ������,������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,H����,����,�κ���,SM490A,��������",
	"���κ� ������,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,����������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,���򺸰���,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,��� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,��� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,�Ϻ� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,�Ϻ� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,������ ������ ���Ʈ,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,������ ������ ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� ������,ä����,�÷���Ʈ,�κ���,SM400B,��������",			// �߰�
	"���κ� ������,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",				// CP_BOLT

	"���κ� �Ϲݺ�,������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,H����,����,�κ���,SM490A,��������",
	"���κ� �Ϲݺ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,����������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,���򺸰���,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,��� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,��� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,�Ϻ� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,�Ϻ� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,������ ������ ���Ʈ,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,������ ������ ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ� �Ϲݺ�,ä����,�÷���Ʈ,�κ���,SM400B,��������",			// �߰�
	"���κ� �Ϲݺ�,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",				// CG_BOLT	

	"���κ�,������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,����������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,���򺸰���,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,��� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,��� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,�Ϻ� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,�Ϻ� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,������ ������,�÷���Ʈ,�κ���,SM400B,��������",
	"���κ�,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",					// ST_BOLT
	
	"�������κ� ������,������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,����������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,���򺸰���,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,��� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,��� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,�Ϻ� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,�Ϻ� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,������ ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� ������,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",			// WI_P_BOLT

	"�������κ� �Ϲݺ�,������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,����������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,���򺸰���,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,��� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,��� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,�Ϻ� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,�Ϻ� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,������ ������,�÷���Ʈ,�κ���,SM400B,��������",
	"�������κ� �Ϲݺ�,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",			// WI_G_BOLT

	"������ ,������,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,����,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,����,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,����� ������,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,����� ����,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,����������,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,���򺸰���,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,��� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,��� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,�Ϻ� ������(���),�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,�Ϻ� ������(�ϸ�),�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,������ ������,�÷���Ʈ,�κ���,SM400B,��������",
	"������ ,��Ʈ&&��Ʈ,��Ʈ,��Ʈ,F10T,��������",
	
	"��Ÿ����,���ܿ�����,����,����,SD30,��������",
	"��Ÿ����,���ܿ�����(����),�÷���Ʈ,�κ���,SM400B,��������",
	"��Ÿ����,SLAB ANCHOR,����,����,SD30,��������",
	"��Ÿ����,���÷���Ʈ,�÷���Ʈ,�ֺ���,SM490B,��������",	
	"��Ÿ����,FACE PLATE,�÷���Ʈ,�ֺ���,SM490B,��������",	
	"��Ÿ����,���,�÷���Ʈ,�ֺ���,SM490B,��������",
	"��Ÿ����,�����,�÷���Ʈ,�ֺ���,SM490B,��������",
	"��Ÿ����,���������,�÷���Ʈ,�ֺ���,SM490B,��������",
	"��Ÿ����,���������(��),�÷���Ʈ,�ֺ���,SM490B,��������",
	"��Ÿ����,���������(��),�÷���Ʈ,�ֺ���,SM490B,��������",
	"��Ÿ����,��� ���÷���Ʈ,�÷���Ʈ,�ֺ���,SM490B,��������",
	"��Ÿ����,��������å,�÷���Ʈ,�κ���,SM400B,��������",
	"��Ÿ����,���������� ���ٸ�,�÷���Ʈ,�κ���,SM400B,��������",	// GI_HG
	};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;
	RemoveAll();

	for(long n = 0; n < nCount; n++)
	{
		if(DivideStr(SArr, szBaseStr[n], ',' )==FALSE) continue;

		// File���� �Է��� �����Ͱ� ���� ��� ó��
		while(SArr.GetSize() < DATASU) SArr.Add(_T(""));

		CSteelSectionData *pData = new CSteelSectionData;

		pData->m_strType	= SArr[0];
		pData->m_strName	= SArr[1];
		pData->m_strShape	= SArr[2];
		pData->m_strClass	= SArr[3];
		pData->m_strSymbol	= SArr[4];
		pData->m_strSizeType= SArr[5];

		m_SectionArr.Add(pData);
	}
}

void	CSteelSection::ClearKey()
{
	m_KeyRec.m_strType   = "";
	m_KeyRec.m_strName   = "";
	m_KeyRec.m_strShape  = "";
	m_KeyRec.m_strClass  = "";
	m_KeyRec.m_strSymbol = "";
	m_KeyRec.m_strSizeType = "";
	

	m_idx	= 0;
	m_Key   = 0;
}

void	CSteelSection::AllocKey(CSteelSectionData* pse, long KeyMode)
{
	ClearKey();
	m_Key = KeyMode;

	if(m_Key & BDISE_TYPE)		m_KeyRec.m_strType		= pse->m_strType;
	if(m_Key & BDISE_NAME)		m_KeyRec.m_strName		= pse->m_strName;
	if(m_Key & BDISE_SHAPE)		m_KeyRec.m_strShape		= pse->m_strShape;
	if(m_Key & BDISE_CLASS)		m_KeyRec.m_strClass		= pse->m_strClass;
	if(m_Key & BDISE_SYMBOL)	m_KeyRec.m_strSymbol	= pse->m_strSymbol;
	if(m_Key & BDISE_SIZETYPE)	m_KeyRec.m_strSizeType	= pse->m_strSizeType;
	
}

BOOL	CSteelSection::StartKey()
{
	long size;
	m_idx = 0;
	
	size = m_SectionArr.GetSize();
	while(m_idx < size) {
		if(m_Key == 0)	return TRUE;

		if(m_Key & BDISE_TYPE) {
			if(m_KeyRec.m_strType != m_SectionArr[m_idx]->m_strType) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_NAME) {
			if(m_KeyRec.m_strName != m_SectionArr[m_idx]->m_strName) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SHAPE) {
			if(m_KeyRec.m_strShape != m_SectionArr[m_idx]->m_strShape) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_CLASS)	{
			if(m_KeyRec.m_strClass != m_SectionArr[m_idx]->m_strClass) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SYMBOL) {
			if(m_KeyRec.m_strSymbol != m_SectionArr[m_idx]->m_strSymbol) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SIZETYPE) {
			if(m_KeyRec.m_strSizeType != m_SectionArr[m_idx]->m_strSizeType) {
				m_idx++;	continue;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL	CSteelSection::NextKey()
{
	long size;
	
	m_idx++;
	size = m_SectionArr.GetSize();
	while(m_idx < size) {
		if(m_Key == 0)	return TRUE;

		if(m_Key & BDISE_TYPE) {
			if(m_KeyRec.m_strType != m_SectionArr[m_idx]->m_strType) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_NAME) {
			if(m_KeyRec.m_strName != m_SectionArr[m_idx]->m_strName) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SHAPE) {
			if(m_KeyRec.m_strShape != m_SectionArr[m_idx]->m_strShape) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_CLASS)	{
			if(m_KeyRec.m_strClass != m_SectionArr[m_idx]->m_strClass) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SYMBOL) {
			if(m_KeyRec.m_strSymbol != m_SectionArr[m_idx]->m_strSymbol) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SIZETYPE) {
			if(m_KeyRec.m_strSizeType != m_SectionArr[m_idx]->m_strSizeType) {
				m_idx++;	continue;
			}
		}
		return TRUE;
	}
	return FALSE;
}

CSteelSectionData*	CSteelSection::GetRecord(CSteelSectionData* pBS)
{
	if(m_idx >= m_SectionArr.GetSize())
	{
		pBS->m_strType	= "";
		pBS->m_strName  = "";
		pBS->m_strShape = "";
		pBS->m_strClass = "";
		pBS->m_strSymbol= "";
		pBS->m_strSizeType= "";
		return NULL;
	}

	*pBS = *m_SectionArr[m_idx];
	return pBS;
}

CString CSteelSection::GetStringData(long idx, SECTION_DATA_ID ID) const
{
	CString str(_T(""));
	if(idx >= m_SectionArr.GetSize())	return str;

	switch(ID) {
	case BDISE_TYPE:
		return m_SectionArr[idx]->m_strType;
	case BDISE_NAME:
		return m_SectionArr[idx]->m_strName;
	case BDISE_SHAPE:
		return m_SectionArr[idx]->m_strShape;
	case BDISE_CLASS:
		return m_SectionArr[idx]->m_strClass;
	case BDISE_SYMBOL:
		return m_SectionArr[idx]->m_strSymbol;
	case BDISE_SIZETYPE:
		return m_SectionArr[idx]->m_strSizeType;
	default:
		str = "Out of Range ID";
	}
	return str;
}

long CSteelSection::GetIndex(const CString& strType, const CString& strName) const
{
	long size = m_SectionArr.GetSize();
	for(long i = 0; i < size; i++) 
	{
		CSteelSectionData* pBS = m_SectionArr[i];
		if (strType == pBS->m_strType && strName == pBS->m_strName) 
			return i;
	}

	return -1;
}

CString	CSteelSection::GetType(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strType;	
}

CString	CSteelSection::GetName(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strName;	
}
CString	CSteelSection::GetShape(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strShape;	
}
CString	CSteelSection::GetClass(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strClass;	
}
CString	CSteelSection::GetBujeSizeType(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strSizeType;	
}
CString	CSteelSection::GetBujeTypeByMarkBuje(long nMarkBuje) const
{
	if(nMarkBuje < 0 || nMarkBuje > m_SectionArr.GetUpperBound()) 
		return _T("");

	return m_SectionArr[nMarkBuje]->m_strClass;
}
CString	CSteelSection::GetMaterialByMarkBuje(long nMarkBuje) const
{
	if(nMarkBuje < 0 || nMarkBuje > m_SectionArr.GetUpperBound()) 
		return _T("");

	return m_SectionArr[nMarkBuje]->m_strSymbol;
}

void CSteelSection::SetType(long idx,const CString& strType  )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strType = strType;		
}
void CSteelSection::SetName(long idx,const CString& strName )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strName = strName;		
}
void CSteelSection::SetShape(long idx,const CString& strShape )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strShape = strShape;		
}
void CSteelSection::SetClass(long idx,const CString& strClass )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strClass = strClass;		
}
void CSteelSection::SetBujeSizeType(long idx,const CString& strSizeType )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strSizeType = strSizeType;		
}

void CSteelSection::SetSymbol(long idx,const	CString& strSymbol)
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strSymbol = strSymbol;	
}
