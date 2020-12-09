// SteelScallop.cpp: implementation of the CSteelScallop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelScallop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSteelScallopData::CSteelScallopData()
{
	m_strType1 = _T("");		//	�ܸ� ���� 1
	m_strName1 = _T("");		//	����� 1
	m_strType2 = _T("");		//	�ܸ� ���� 2
	m_strName2 = _T("");		//	����� 2
	m_strType3 = _T("");		//	�ܸ� ���� 3
	m_strName3 = _T("");		//	����� 3

	m_nCutType = 0;				//	����� Ÿ��	
}

CSteelScallopData::~CSteelScallopData()
{

}

void CSteelScallopData::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_strType1;
		ar << m_strName1;
		ar << m_strType2;
		ar << m_strName2;		
		ar << m_strType3;
		ar << m_strName3;		

		ar << m_nCutType;				
	}
	else
	{
		ar >> nFlag;
		ar >> m_strType1;
		ar >> m_strName1;
		ar >> m_strType2;
		ar >> m_strName2;		
		ar >> m_strType3;
		ar >> m_strName3;		

		ar >> m_nCutType;				
	}
}

CSteelScallopData& CSteelScallopData::operator=(const CSteelScallopData& obj)
{
	if(this == &obj) return *this;

	m_strType1		= obj.m_strType1;		
	m_strName1		= obj.m_strName1;		
	m_strType2		= obj.m_strType2;		
	m_strName2		= obj.m_strName2;		
	m_strType3		= obj.m_strType3;		
	m_strName3		= obj.m_strName3;		
	m_nCutType		= obj.m_nCutType;		
	
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelScallop::CSteelScallop()
{
	DataInit();
}

CSteelScallop::~CSteelScallop()
{
	RemoveAll();
}

void CSteelScallop::RemoveAll()
{	
	for(long n = 0; n < m_SectionArr.GetSize(); n++)
		delete m_SectionArr[n];
	m_SectionArr.RemoveAll();

}
void CSteelScallop::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		AhTPASerial(ar, &m_SectionArr, (CSteelScallopData *)0);
	}
	else
	{		
		ar >> nFlag;
		AhTPASerial(ar, &m_SectionArr, (CSteelScallopData *)0);
	}
}

long CSteelScallop::GetItemValue(CString Text)
{
	if(Text == "��Ķ��")	return 0;
	if(Text == "ä��")		return 1;
	if(Text == "�����")	return 2;
	if(Text == "����")		return 3;
	ASSERT(FALSE);
	return -1;
}
 
CString CSteelScallop::GetCutTypeByIndex(long nIdx)
{
	switch(nIdx)
	{
	case 0: return "��Ķ��";	break;
	case 1: return "ä��";		break;
	case 2: return "�����";	break;
	case 3: return "����";		break;
	default: ASSERT(FALSE); break;
	}
	return "";
}

void CSteelScallop::DataInit()
{	
	//long nAdd = 2;

	char* szBaseStr[] = {	
		"�Ϲ�, ����, �Ϲ�, ����,   �Ϲ�, ������,    ��Ķ��",//070207���� 2�� �׸� �߰�......KB....
		"�Ϲ�, ����, �Ϲ�, ����,   �Ϲ�, ������,    ��Ķ��",

		"�Ϲ�, ����, �Ϲ�, ������, �Ϲ�, ����������,��Ķ��",
		"�Ϲ�, ����, �Ϲ�, ������, �Ϲ�, ����������,��Ķ��",
		
		"���κ� ������,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",	// 2 + nAdd
		"���κ� ������,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"���κ� ������,����� ������,�Ϲ� ,������,���κ� ������,����� ����,��Ķ��",
		"���κ� ������,����� ������,�Ϲ� ,������,���κ� ������,����� ����,��Ķ��",
		"���κ� ������,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"���κ� ������,����������,���κ� ������,����� ������,���κ� ������,����� ����,��Ķ��",
		"���κ� ������,����������,���κ� ������,����� ������,���κ� ������,����� ����,��Ķ��",
		"���κ� ������,����� ����,�Ϲ� ,����������,�Ϲ� ,������,��Ķ��",
		"���κ� ������,����� ����,�Ϲ� ,����������,�Ϲ� ,������,��Ķ��",

		"���κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"���κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"���κ� �Ϲݺ�,����� ������,�Ϲ� ,������,���κ� �Ϲݺ�,����� ����,��Ķ��",
		"���κ� �Ϲݺ�,����� ������,�Ϲ� ,������,���κ� �Ϲݺ�,����� ����,��Ķ��",
		"���κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"���κ� �Ϲݺ�,����������,���κ� �Ϲݺ�,����� ������,���κ� �Ϲݺ�,����� ����,��Ķ��",
		"���κ� �Ϲݺ�,����������,���κ� �Ϲݺ�,����� ������,���κ� �Ϲݺ�,����� ����,��Ķ��",
		"���κ� �Ϲݺ�,����� ����,�Ϲ� ,����������,�Ϲ� ,������,��Ķ��",
		"���κ� �Ϲݺ�,����� ����,�Ϲ� ,����������,�Ϲ� ,������,��Ķ��",
		
		"���κ�,����� ������,���κ� �Ϲݺ�,������,���κ� �Ϲݺ�,����,��Ķ��",	// 20 + nAdd
		"���κ�,����� ������,���κ� �Ϲݺ�,������,���κ� �Ϲݺ�,����,��Ķ��",
		"���κ�,����� ������,���κ� �Ϲݺ�,������,���κ�,����� ����,��Ķ��",
		"���κ�,����� ������,���κ� �Ϲݺ�,������,���κ�,����� ����,��Ķ��",
		"���κ�,����� ������,���κ� �Ϲݺ�,������,���κ� �Ϲݺ�,����,�����",
		"���κ�,����������,���κ�,������,���κ�,����,��Ķ��",
		"���κ�,����������,���κ�,������,���κ�,����,��Ķ��",	

		"�������κ� ������,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��", // 27 + nAdd
		"�������κ� ������,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"�������κ� ������,����� ������,�Ϲ� ,������,�������κ� ������,����� ����,��Ķ��",
		"�������κ� ������,����� ������,�Ϲ� ,������,�������κ� ������,����� ����,��Ķ��",
		"�������κ� ������,����� ������,�Ϲ� ,������,�Ϲ� ,����,�����",
		"�������κ� ������,����������,�������κ� ������,����� ������,�������κ� ������,����� ����,��Ķ��",
		"�������κ� ������,����������,�������κ� ������,����� ������,�������κ� ������,����� ����,��Ķ��",
		
		"�������κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"�������κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"�������κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�������κ� �Ϲݺ�,����� ����,��Ķ��",
		"�������κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�������κ� �Ϲݺ�,����� ����,��Ķ��",
		"�������κ� �Ϲݺ�,����� ������,�Ϲ� ,������,�Ϲ� ,����,�����",
		"�������κ� �Ϲݺ�,����������,�������κ� �Ϲݺ�,����� ������,�������κ� �Ϲݺ�,����� ����,��Ķ��",
		"�������κ� �Ϲݺ�,����������,�������κ� �Ϲݺ�,����� ������,�������κ� �Ϲݺ�,����� ����,��Ķ��",
		
		"������ ,����������,������ ,������,������ ,����,��Ķ��", // 41 + nAdd
		"������ ,����������,������ ,������,������ ,����,��Ķ��",		
		
		"����극�̽�,������,���κ� �Ϲݺ�,����� ������,�Ϲ�,������,��Ķ��", //43 + nAdd
		"����극�̽�,������,���κ� ������,����� ������,�Ϲ�,������,��Ķ��", 

		"��Ÿ����,���ܿ�����,,,,,��Ķ��",//45 + nAdd
		"��Ÿ����,SLAB ANCHOR,,,,,��Ķ��",		
		"��Ÿ����,���÷���Ʈ,,,,,��Ķ��",
		"��Ÿ����,���,,,,,��Ķ��",
		"��Ÿ����,���������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"��Ÿ����,���������,�Ϲ� ,������,�Ϲ� ,����,��Ķ��",
		"��Ÿ����,��� ���÷���Ʈ,,,,,��Ķ��",
		"��Ÿ����,��������å,,,,,��Ķ��",
		"��Ÿ����,���������� ���ٸ�,,,,,��Ķ��",
	};
	
	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;
	RemoveAll();
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;
		
		CSteelScallopData *pData = new CSteelScallopData;

		pData->m_strType1	= SArr[0];
		pData->m_strName1	= SArr[1];
		pData->m_strType2	= SArr[2];
		pData->m_strName2	= SArr[3];
		pData->m_strType3	= SArr[4];
		pData->m_strName3	= SArr[5];
		pData->m_nCutType	= GetItemValue(SArr[6]);
		
		m_SectionArr.Add(pData);
	}
}


CSteelScallopData* CSteelScallop::GetBaseScallopByIndex(long nIndex)
{
	if(m_SectionArr.GetSize() == 0)			return NULL;
	if(m_SectionArr.GetSize() <= nIndex)	return NULL;

	return m_SectionArr[nIndex];
}
