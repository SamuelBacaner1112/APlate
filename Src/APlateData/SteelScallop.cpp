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
	m_strType1 = _T("");		//	단면 형식 1
	m_strName1 = _T("");		//	부재명 1
	m_strType2 = _T("");		//	단면 형식 2
	m_strName2 = _T("");		//	부재명 2
	m_strType3 = _T("");		//	단면 형식 3
	m_strName3 = _T("");		//	부재명 3

	m_nCutType = 0;				//	모따기 타입	
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
	if(Text == "스캘럽")	return 0;
	if(Text == "채움")		return 1;
	if(Text == "모따기")	return 2;
	if(Text == "없음")		return 3;
	ASSERT(FALSE);
	return -1;
}
 
CString CSteelScallop::GetCutTypeByIndex(long nIdx)
{
	switch(nIdx)
	{
	case 0: return "스캘럽";	break;
	case 1: return "채움";		break;
	case 2: return "모따기";	break;
	case 3: return "없음";		break;
	default: ASSERT(FALSE); break;
	}
	return "";
}

void CSteelScallop::DataInit()
{	
	//long nAdd = 2;

	char* szBaseStr[] = {	
		"일반, 상판, 일반, 상판,   일반, 복부판,    스캘럽",//070207일자 2개 항목 추가......KB....
		"일반, 하판, 일반, 하판,   일반, 복부판,    스캘럽",

		"일반, 상판, 일반, 복부판, 일반, 수직보강재,스캘럽",
		"일반, 하판, 일반, 복부판, 일반, 수직보강재,스캘럽",
		
		"가로보 지점부,브라켓 복부판,일반 ,복부판,일반 ,상판,스캘럽",	// 2 + nAdd
		"가로보 지점부,브라켓 복부판,일반 ,복부판,일반 ,하판,스캘럽",
		"가로보 지점부,브라켓 복부판,일반 ,복부판,가로보 지점부,브라켓 하판,스캘럽",
		"가로보 지점부,브라켓 보강재,일반 ,복부판,가로보 지점부,브라켓 하판,스캘럽",
		"가로보 지점부,브라켓 보강재,일반 ,복부판,일반 ,하판,스캘럽",
		"가로보 지점부,수직보강재,가로보 지점부,브라켓 복부판,가로보 지점부,브라켓 상판,스캘럽",
		"가로보 지점부,수직보강재,가로보 지점부,브라켓 복부판,가로보 지점부,브라켓 하판,스캘럽",
		"가로보 지점부,브라켓 상판,일반 ,수직보강재,일반 ,복부판,스캘럽",
		"가로보 지점부,브라켓 하판,일반 ,수직보강재,일반 ,복부판,스캘럽",

		"가로보 일반부,브라켓 복부판,일반 ,복부판,일반 ,상판,스캘럽",
		"가로보 일반부,브라켓 복부판,일반 ,복부판,일반 ,하판,스캘럽",
		"가로보 일반부,브라켓 복부판,일반 ,복부판,가로보 일반부,브라켓 하판,스캘럽",
		"가로보 일반부,브라켓 보강재,일반 ,복부판,가로보 일반부,브라켓 하판,스캘럽",
		"가로보 일반부,브라켓 보강재,일반 ,복부판,일반 ,하판,스캘럽",
		"가로보 일반부,수직보강재,가로보 일반부,브라켓 복부판,가로보 일반부,브라켓 상판,스캘럽",
		"가로보 일반부,수직보강재,가로보 일반부,브라켓 복부판,가로보 일반부,브라켓 하판,스캘럽",
		"가로보 일반부,브라켓 상판,일반 ,수직보강재,일반 ,복부판,스캘럽",
		"가로보 일반부,브라켓 하판,일반 ,수직보강재,일반 ,복부판,스캘럽",
		
		"세로보,브라켓 복부판,가로보 일반부,복부판,가로보 일반부,상판,스캘럽",	// 20 + nAdd
		"세로보,브라켓 복부판,가로보 일반부,복부판,가로보 일반부,하판,스캘럽",
		"세로보,브라켓 복부판,가로보 일반부,복부판,세로보,브라켓 상판,스캘럽",
		"세로보,브라켓 보강재,가로보 일반부,복부판,세로보,브라켓 하판,스캘럽",
		"세로보,브라켓 보강재,가로보 일반부,복부판,가로보 일반부,하판,모따기",
		"세로보,수직보강재,세로보,복부판,세로보,상판,스캘럽",
		"세로보,수직보강재,세로보,복부판,세로보,하판,스캘럽",	

		"외측가로보 지점부,브라켓 복부판,일반 ,복부판,일반 ,상판,스캘럽", // 27 + nAdd
		"외측가로보 지점부,브라켓 복부판,일반 ,복부판,일반 ,하판,스캘럽",
		"외측가로보 지점부,브라켓 복부판,일반 ,복부판,외측가로보 지점부,브라켓 하판,스캘럽",
		"외측가로보 지점부,브라켓 보강재,일반 ,복부판,외측가로보 지점부,브라켓 하판,스캘럽",
		"외측가로보 지점부,브라켓 보강재,일반 ,복부판,일반 ,하판,모따기",
		"외측가로보 지점부,수직보강재,외측가로보 지점부,브라켓 복부판,외측가로보 지점부,브라켓 상판,스캘럽",
		"외측가로보 지점부,수직보강재,외측가로보 지점부,브라켓 복부판,외측가로보 지점부,브라켓 하판,스캘럽",
		
		"외측가로보 일반부,브라켓 복부판,일반 ,복부판,일반 ,상판,스캘럽",
		"외측가로보 일반부,브라켓 복부판,일반 ,복부판,일반 ,하판,스캘럽",
		"외측가로보 일반부,브라켓 복부판,일반 ,복부판,외측가로보 일반부,브라켓 하판,스캘럽",
		"외측가로보 일반부,브라켓 보강재,일반 ,복부판,외측가로보 일반부,브라켓 하판,스캘럽",
		"외측가로보 일반부,브라켓 보강재,일반 ,복부판,일반 ,하판,모따기",
		"외측가로보 일반부,수직보강재,외측가로보 일반부,브라켓 복부판,외측가로보 일반부,브라켓 상판,스캘럽",
		"외측가로보 일반부,수직보강재,외측가로보 일반부,브라켓 복부판,외측가로보 일반부,브라켓 하판,스캘럽",
		
		"외측빔 ,수직보강재,외측빔 ,복부판,외측빔 ,상판,스캘럽", // 41 + nAdd
		"외측빔 ,수직보강재,외측빔 ,복부판,외측빔 ,하판,스캘럽",		
		
		"수평브레이싱,이음판,가로보 일반부,브라켓 복부판,일반,복부판,스캘럽", //43 + nAdd
		"수평브레이싱,이음판,가로보 지점부,브라켓 복부판,일반,복부판,스캘럽", 

		"기타부재,전단연결재,,,,,스캘럽",//45 + nAdd
		"기타부재,SLAB ANCHOR,,,,,스캘럽",		
		"기타부재,솔플레이트,,,,,스캘럽",
		"기타부재,들고리,,,,,스캘럽",
		"기타부재,잭업보강재,일반 ,복부판,일반 ,상판,스캘럽",
		"기타부재,잭업보강재,일반 ,복부판,일반 ,하판,스캘럽",
		"기타부재,잭업 솔플레이트,,,,,스캘럽",
		"기타부재,낙교방지책,,,,,스캘럽",
		"기타부재,유지보수용 동바리,,,,,스캘럽",
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
