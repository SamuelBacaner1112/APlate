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
	if	   ( cs == "거더NO" )	nFlag = 0;
	else if( cs == "SP.NO")		nFlag = 1;
	else if( cs == "단면문자")	nFlag = 2;
	else if( cs == "단면NO")	nFlag = 3;
	else if( cs == "TAG1")		nFlag = 4;
	else if( cs == "TAG2")		nFlag = 5;
	else if( cs == "TAG3")		nFlag = 6;
	else if( cs == "TAG4")		nFlag = 7;
	else if( cs == "Sr. 1")		nFlag = 8;
	else if( cs == "Sr. A")		nFlag = 9;
	else if( cs == "Sr. a")		nFlag = 10;
	else if( cs == "Sr. A 간략")nFlag = 11;
	else if( cs == "Sr. a 간략")nFlag = 12;
	else if( cs == "(-)")		nFlag = 13;
	else if( cs == "두께")		nFlag = 14;
	else if( cs == "길이")		nFlag = 15;
	else if( cs == "재질")		nFlag = 16;
	else if( cs == "없음")		nFlag = 17;	

	return nFlag;
}

long CSteelMark::GetShopSerialType(CString cs)
{	
	long	nFlag = -1;

	if	   ( cs == "SPLICE" )	nFlag = 0;
	else if( cs == "거더")		nFlag = 1;
	else if( cs == "BLOCK")		nFlag = 2;
	else if( cs == "현재교량")	nFlag = 3;
	else if( cs == "여러교량블력")	nFlag = 4;
	else if( cs == "여러교량전체")	nFlag = 5;
	
	return nFlag;
}

CString CSteelMark::GetShopSerialType(long nType)
{	
	ASSERT(nType >= 0 || nType < 6);

	CString cs;
	switch (nType)
	{
	case 0: cs = "SPLICE";	break;
	case 1: cs = "거더";	break;
	case 2: cs = "BLOCK";	break;
	case 3: cs = "현재교량";	break;
	case 4: cs = "여러교량블력";	break;
	case 5: cs = "여러교량전체";	break;	
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
	case 0:	cs = "거더NO";		break;
	case 1: cs = "SP.NO";		break;
	case 2: cs = "단면문자";	break;
	case 3: cs = "단면NO";		break;
	case 4: cs = "TAG1";		break;
	case 5: cs = "TAG2";		break;
	case 6: cs = "TAG3";		break;
	case 7: cs = "TAG4";		break;
	case 8: cs = "Sr. 1";		break;
	case 9: cs = "Sr. A";		break;
	case 10: cs = "Sr. a";		break;
	case 11: cs = "Sr. A 간략";	break;
	case 12: cs = "Sr. a 간략"; break;
	case 13: cs = "(-)";		break;	
	case 14: cs = "두께";		break;
	case 15: cs = "길이";		break;
	case 16: cs = "재질";		break;
	case 17: cs = "없음";		break;
	default: cs = _T("");		break;
	}

	return cs;
}

long CSteelMark::GetShopSrBase(CString cs)
{	
	long	nFlag = -1;

	if	   ( cs == "순서대로" )			nFlag = 0;
	else if( cs == "사용 빈도순(대)")	nFlag = 1;
	else if( cs == "사용 빈도순(소)")	nFlag = 2;
	else if( cs == "길이별 순서(대)")	nFlag = 3;
	else if( cs == "길이별 순서(소)")	nFlag = 4;	
	
	return nFlag;
}

CString CSteelMark::GetShopSrBase(long nType)
{
	ASSERT(nType >= 0 || nType < 5);

	CString cs;
	switch (nType)
	{
	case 0: cs = "순서대로";	break;
	case 1: cs = "사용 빈도순(대)";	break;
	case 2: cs = "사용 빈도순(소)";	break;
	case 3: cs = "길이별 순서(대)";	break;
	case 4: cs = "길이별 순서(소)";	break;	
	}	
	return cs;
}

void CSteelMark::SetDataWeldInit()
{
	char* szBaseStr[] = {
		"일반,상판,일반,상판,#T, , , ,SPLICE,TAG1,두께,두께 , , , , , , , ,1,순서대로",
		"일반,하판,일반,하판,#B, , , ,SPLICE,TAG1,두께,두께, , , , , , , ,1,순서대로",
		"일반,복부판,일반,복부판,#W, , , ,SPLICE,TAG1,두께,두께 , , , , , , , ,1,순서대로"
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
		"A,SECTION, , , , ,현재교량,단면문자, , , , , , , , , ,1,길이별 순서(대)",
		"P,SECTION, , , , ,현재교량,단면문자, , , , , , , , , ,1,길이별 순서(대)",
		"CU,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"CL,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"CX,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"B,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"D,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"E,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"F,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"G,SECTION, , , , ,현재교량,단면문자,단면NO, , , , , , , , ,1,길이별 순서(대)",
		"현장이음,SECTION,SP, , , ,거더,TAG1,단면NO, , , , , , , , ,1,순서대로",
		"상판,SECTION,TF, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"하판,SECTION,BF, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"좌측복부판,SECTION,LW, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"우측복부판,SECTION,RW, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"세로보,SECTION,STR, , , ,거더,거더NO,TAG1,Sr. 1, , , , , , , ,1,순서대로",
		"가로보,SECTION,CR, , , ,거더,거더NO,TAG1,Sr. 1, , , , , , , ,1,순서대로",
		"외측가로보,SECTION,WG, , , ,거더,거더NO,TAG1,Sr. 1, , , , , , , ,1,순서대로",
		"외측빔,SECTION,CS, , , ,거더,거더NO,TAG1,Sr. 1, , , , , , , ,1,순서대로",
		"내부출입구,SECTION,DOR, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"맨홀,SECTION,MH, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"환기구,SECTION,FP, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"시점교대,SECTION,BR,S, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"종점교대,SECTION,BR,E, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"시점슬래브,SECTION,SLAB,S, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"종점슬래브,SECTION,SLAB,E, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"시점거더,SECTION,BOX,S, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"종점거더,SECTION,BOX,E, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"시작 TAPER,SECTION,B/L,S, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"종점 TAPER,SECTION,B/L,E, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"시작 CURVE,SECTION,B/L,S, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"종점 CURVE,SECTION,B/L,E, , ,거더,TAG1,(-),TAG2, , , , , , , ,1,순서대로",
		"잭업단면,SECTION,J, , , ,거더,TAG1,Sr. 1, , , , , , , , ,1,순서대로",
		"평면선형,SECTION,BC, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 EC,SECTION,EC, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 BTC,SECTION,BTC, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 ETC,SECTION,ETC, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 KAA,SECTION,KAA, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 KAE,SECTION,KAE, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 BVC,SECTION,BVC, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 EVC,SECTION,EVC, , , ,거더,TAG1, , , , , , , , , ,1,순서대로",
		"평면선형 SLOPE,SECTION,SUPER, , , ,거더,TAG1, , , , , , , , , ,1,순서대로"
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
