// SteelMark.cpp: implementation of the CSteelMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelMark.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelMarkData::CSteelMarkData()
{
	m_nStartNo = 0;
	m_nSerialType = 0;
	m_nSrBase = 0;
}

CSteelMarkData::~CSteelMarkData()
{

}

void CSteelMarkData::Serialize(CArchive& ar)
{	
	long nFlag = 0;
	int	i = 0;
	if(ar.IsStoring()) 
	{
		ar << nFlag;
		for(i=0;i<4;i++)
			ar << m_sMark[i];
		for(i=0;i<10;i++)
			ar << m_nMark[i];
		ar << m_nStartNo;
		ar << m_nSerialType;
		ar << m_nSrBase;
	}
	else 
	{
		ar >> nFlag;
		for(i=0;i<4;i++)
			ar >> m_sMark[i];
		for(i=0;i<10;i++)
			ar >> m_nMark[i];		
		ar >> m_nStartNo;
		ar >> m_nSerialType;
		ar >> m_nSrBase;
	}
}

CSteelMarkData& CSteelMarkData::operator=(const CSteelMarkData& obj)
{
	if(this==&obj) return *this;

	int i;

	for(i=0;i<4;i++)
		m_sMark[i] = obj.m_sMark[i];
	for(i=0;i<10;i++)
		m_nMark[i] = obj.m_nMark[i];
	m_nStartNo = obj.m_nStartNo;
	m_nSerialType = obj.m_nSerialType;
	m_nSrBase = obj.m_nSrBase;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelMark::CSteelMark()
{		
	SetDataSectionInit();
	SetDataWeldInit();
}

CSteelMark::~CSteelMark()
{
	RemoveAll(0);
	RemoveAll(1);	
	RemoveAll(2);	
	RemoveAll(3);
	RemoveAll(4);
	RemoveAll(5);
}

void CSteelMark::Serialize(CArchive& ar)
{	
	//AhTPASerial(ar, &m_PieceMarkArr, (CSteelMarkData *)0);	
	AhTPASerial(ar, &m_SectionMarkArr, (CSteelMarkData *)0);		
	//AhTPASerial(ar, &m_AssemblyMarkArr, (CSteelMarkData *)0);
	//AhTPASerial(ar, &m_MidAssemblyMarkArr, (CSteelMarkData *)0);
	//AhTPASerial(ar, &m_BigAssemblyMarkArr, (CSteelMarkData *)0);
	AhTPASerial(ar, &m_WeldMarkArr, (CSteelMarkData *)0);
}

long CSteelMark::GetShopBaseMark(CString cs)
{	
	long	nFlag = -1;
	if	   ( cs == "�Ŵ�NO" )	nFlag = 0;
	else if( cs == "SP.NO")		nFlag = 1;
	else if( cs == "�ܸ鹮��")	nFlag = 2;
	else if( cs == "�ܸ�NO")	nFlag = 3;
	else if( cs == "TAG1")		nFlag = 4;
	else if( cs == "TAG2")		nFlag = 5;
	else if( cs == "TAG3")		nFlag = 6;
	else if( cs == "TAG4")		nFlag = 7;
	else if( cs == "Sr. 1")		nFlag = 8;
	else if( cs == "Sr. A")		nFlag = 9;
	else if( cs == "Sr. a")		nFlag = 10;
	else if( cs == "Sr. A ����")nFlag = 11;
	else if( cs == "Sr. a ����")nFlag = 12;
	else if( cs == "(-)")		nFlag = 13;
	else if( cs == "�β�")		nFlag = 14;
	else if( cs == "����")		nFlag = 15;
	else if( cs == "����")		nFlag = 16;
	else if( cs == "����")		nFlag = 17;	

	return nFlag;
}

long CSteelMark::GetShopSerialType(CString cs)
{	
	long	nFlag = -1;

	if	   ( cs == "SPLICE" )	nFlag = 0;
	else if( cs == "�Ŵ�")		nFlag = 1;
	else if( cs == "BLOCK")		nFlag = 2;
	else if( cs == "���米��")	nFlag = 3;
	else if( cs == "�����������")	nFlag = 4;
	else if( cs == "����������ü")	nFlag = 5;
	
	return nFlag;
}

CString CSteelMark::GetShopSerialType(long nType)
{	
	ASSERT(nType >= 0 || nType < 6);

	CString cs;
	switch (nType)
	{
	case 0: cs = "SPLICE";	break;
	case 1: cs = "�Ŵ�";	break;
	case 2: cs = "BLOCK";	break;
	case 3: cs = "���米��";	break;
	case 4: cs = "�����������";	break;
	case 5: cs = "����������ü";	break;	
	}	
	return cs;
}

CString CSteelMark::GetShopBaseMark(long nRow, long nCol, long nType)
{
	if( nCol == 4) return "";
	ASSERT(nType >= 0 || nType < 3);
	if( nCol < 4 )
	{
		if (nType ==0) return m_PieceMarkArr[nRow]->m_sMark[nCol];
		if (nType ==1) return m_SectionMarkArr[nRow]->m_sMark[nCol];
		if (nType ==2) return m_AssemblyMarkArr[nRow]->m_sMark[nCol];
		if (nType ==3) return m_WeldMarkArr[nRow]->m_sMark[nCol];
		if (nType ==4) return m_MidAssemblyMarkArr[nRow]->m_sMark[nCol];
		if (nType ==5) return m_BigAssemblyMarkArr[nRow]->m_sMark[nCol];
	}
	long	nFlag;
	if (nType ==0) nFlag = m_PieceMarkArr[nRow]->m_nMark[nCol-5];
	if (nType ==1) nFlag = m_SectionMarkArr[nRow]->m_nMark[nCol-5];
	if (nType ==2) nFlag = m_AssemblyMarkArr[nRow]->m_nMark[nCol-5];
	if (nType ==3) nFlag = m_WeldMarkArr[nRow]->m_nMark[nCol-5];
	if (nType ==4) nFlag = m_MidAssemblyMarkArr[nRow]->m_nMark[nCol-5];
	if (nType ==5) nFlag = m_BigAssemblyMarkArr[nRow]->m_nMark[nCol-5];
	CString cs;
	switch( nFlag )
	{
	case 0:	cs = "�Ŵ�NO";		break;
	case 1: cs = "SP.NO";		break;
	case 2: cs = "�ܸ鹮��";	break;
	case 3: cs = "�ܸ�NO";		break;
	case 4: cs = "TAG1";		break;
	case 5: cs = "TAG2";		break;
	case 6: cs = "TAG3";		break;
	case 7: cs = "TAG4";		break;
	case 8: cs = "Sr. 1";		break;
	case 9: cs = "Sr. A";		break;
	case 10: cs = "Sr. a";		break;
	case 11: cs = "Sr. A ����";	break;
	case 12: cs = "Sr. a ����"; break;
	case 13: cs = "(-)";		break;	
	case 14: cs = "�β�";		break;
	case 15: cs = "����";		break;
	case 16: cs = "����";		break;
	case 17: cs = "����";		break;
	default: cs = _T("");		break;
	}

	return cs;
}

long CSteelMark::GetShopSrBase(CString cs)
{	
	long	nFlag = -1;

	if	   ( cs == "�������" )			nFlag = 0;
	else if( cs == "��� �󵵼�(��)")	nFlag = 1;
	else if( cs == "��� �󵵼�(��)")	nFlag = 2;
	else if( cs == "���̺� ����(��)")	nFlag = 3;
	else if( cs == "���̺� ����(��)")	nFlag = 4;	
	
	return nFlag;
}

CString CSteelMark::GetShopSrBase(long nType)
{
	ASSERT(nType >= 0 || nType < 5);

	CString cs;
	switch (nType)
	{
	case 0: cs = "�������";	break;
	case 1: cs = "��� �󵵼�(��)";	break;
	case 2: cs = "��� �󵵼�(��)";	break;
	case 3: cs = "���̺� ����(��)";	break;
	case 4: cs = "���̺� ����(��)";	break;	
	}	
	return cs;
}

void CSteelMark::SetDataWeldInit()
{
	char* szBaseStr[] = {
		"�Ϲ�,����,�Ϲ�,����,#T, , , ,SPLICE,TAG1,�β�,�β� , , , , , , , ,1,�������",
		"�Ϲ�,����,�Ϲ�,����,#B, , , ,SPLICE,TAG1,�β�,�β�, , , , , , , ,1,�������",
		"�Ϲ�,������,�Ϲ�,������,#W, , , ,SPLICE,TAG1,�β�,�β� , , , , , , , ,1,�������"
	};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;	
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;

		CSteelMarkData *pMark = new CSteelMarkData;

		long nSerial = 2;	
		long i = 0;
		for(i = 0;i < 4;i++)
			pMark->m_sMark[i]					= SArr[nSerial++];
		pMark->m_nSerialType					= GetShopSerialType(SArr[nSerial++]);
		for(i = 0;i < 10;i++)
			pMark->m_nMark[i]					= GetShopBaseMark(SArr[nSerial++]);
		pMark->m_nStartNo						= atol(SArr[nSerial++]);
		pMark->m_nSrBase						= GetShopSrBase(SArr[nSerial++]);
	
		m_SectionMarkArr.Add(pMark);				
	}
}

void CSteelMark::SetDataSectionInit()
{					
	char* szBaseStr[] = {
		"A,SECTION, , , , ,���米��,�ܸ鹮��, , , , , , , , , ,1,���̺� ����(��)",
		"P,SECTION, , , , ,���米��,�ܸ鹮��, , , , , , , , , ,1,���̺� ����(��)",
		"CU,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"CL,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"CX,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"B,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"D,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"E,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"F,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"G,SECTION, , , , ,���米��,�ܸ鹮��,�ܸ�NO, , , , , , , , ,1,���̺� ����(��)",
		"��������,SECTION,SP, , , ,�Ŵ�,TAG1,�ܸ�NO, , , , , , , , ,1,�������",
		"����,SECTION,TF, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"����,SECTION,BF, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"����������,SECTION,LW, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"����������,SECTION,RW, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"���κ�,SECTION,STR, , , ,�Ŵ�,�Ŵ�NO,TAG1,Sr. 1, , , , , , , ,1,�������",
		"���κ�,SECTION,CR, , , ,�Ŵ�,�Ŵ�NO,TAG1,Sr. 1, , , , , , , ,1,�������",
		"�������κ�,SECTION,WG, , , ,�Ŵ�,�Ŵ�NO,TAG1,Sr. 1, , , , , , , ,1,�������",
		"������,SECTION,CS, , , ,�Ŵ�,�Ŵ�NO,TAG1,Sr. 1, , , , , , , ,1,�������",
		"�������Ա�,SECTION,DOR, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"��Ȧ,SECTION,MH, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"ȯ�ⱸ,SECTION,FP, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"��������,SECTION,BR,S, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"��������,SECTION,BR,E, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"����������,SECTION,SLAB,S, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"����������,SECTION,SLAB,E, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"�����Ŵ�,SECTION,BOX,S, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"�����Ŵ�,SECTION,BOX,E, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"���� TAPER,SECTION,B/L,S, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"���� TAPER,SECTION,B/L,E, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"���� CURVE,SECTION,B/L,S, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"���� CURVE,SECTION,B/L,E, , ,�Ŵ�,TAG1,(-),TAG2, , , , , , , ,1,�������",
		"����ܸ�,SECTION,J, , , ,�Ŵ�,TAG1,Sr. 1, , , , , , , , ,1,�������",
		"��鼱��,SECTION,BC, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� EC,SECTION,EC, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� BTC,SECTION,BTC, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� ETC,SECTION,ETC, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� KAA,SECTION,KAA, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� KAE,SECTION,KAE, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� BVC,SECTION,BVC, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� EVC,SECTION,EVC, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������",
		"��鼱�� SLOPE,SECTION,SUPER, , , ,�Ŵ�,TAG1, , , , , , , , , ,1,�������"
	};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;	
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;

		CSteelMarkData *pMark = new CSteelMarkData;

		long nSerial = 2;	
		long i = 0;
		for(i = 0;i < 4;i++)
			pMark->m_sMark[i]					= SArr[nSerial++];
		pMark->m_nSerialType					= GetShopSerialType(SArr[nSerial++]);
		for(i = 0;i < 10;i++)
			pMark->m_nMark[i]					= GetShopBaseMark(SArr[nSerial++]);
		pMark->m_nStartNo						= atol(SArr[nSerial++]);
		pMark->m_nSrBase						= GetShopSrBase(SArr[nSerial++]);
	
		m_SectionMarkArr.Add(pMark);				
	}
}

void CSteelMark::RemoveAll(long nType)
{
	if(nType == 0)	AhTPADelete(&m_PieceMarkArr, (CSteelMarkData *)0);	
	if(nType == 1)	AhTPADelete(&m_SectionMarkArr, (CSteelMarkData *)0);		
	if(nType == 2)	AhTPADelete(&m_AssemblyMarkArr, (CSteelMarkData *)0);
	if(nType == 3)	AhTPADelete(&m_WeldMarkArr, (CSteelMarkData *)0);
	if(nType == 4)	AhTPADelete(&m_MidAssemblyMarkArr, (CSteelMarkData *)0);
	if(nType == 5)	AhTPADelete(&m_BigAssemblyMarkArr, (CSteelMarkData *)0);
}
