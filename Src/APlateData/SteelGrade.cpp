// SteelGrade.cpp: implementation of the CSteelGrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelGrade.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define DATASU	8

CSteelGrade::CSteelGrade()
{	
	SetDataInit();
}

CSteelGrade::~CSteelGrade()
{
	RemoveAll();
}

void CSteelGrade::RemoveAll()
{
	long size = m_GradeArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_GradeArr[n];
	m_GradeArr.RemoveAll();
}

void CSteelGrade::Serialize(CArchive& ar)
{
	long size=0;
	long nFlag = 0;
	if(ar.IsStoring()) 
	{
		ar << nFlag;
		size = 	m_GradeArr.GetSize();
		ar << size;
		for(long n = 0 ; n < m_GradeArr.GetSize() ; n++)
			m_GradeArr[n]->Serialize(ar);
	}
	else 
	{
		ar >> nFlag;
		RemoveAll();
		ar >> size;
		for(long n = 0 ; n < size ; n++)
		{
			CSteelGradeData *pData = new CSteelGradeData;
			pData->Serialize(ar);
			m_GradeArr.Add(pData);
		}
	}
}
	
void CSteelGrade::SetDataInit()
{
	RemoveAll();
	char* szBaseStr[] = {
		"1등급,  240000,  432000,  24000,  96000,  12.7,  108000,  156000",
		"2등급,  180000,  324000,  18000,  72000,  09.5,   81000,  117000",
		"3등급,  135000,  243000,  13500,  54000,  07.1,   60800,   87800",		
	};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;
		
		CSteelGradeData *pData = new CSteelGradeData;

		pData->m_strGrade	=	SArr[0];
		pData->m_weDB		=	atof(SArr[1]);
		pData->m_weDBPt		=	atof(SArr[2]);
		pData->m_weDBPf		=	atof(SArr[3]);
		pData->m_weDBPr		=	atof(SArr[4]);
		pData->m_weDL		=	atof(SArr[5]);
		pData->m_weDLPm		=	atof(SArr[6]);
		pData->m_weDLPs		=	atof(SArr[7]);

		m_GradeArr.Add(pData);
	}
}

long	CSteelGrade::GetIndex(CString strGrade)
{
	long idx, size;

	idx = 0;
	size = GetSize();
	while(idx < size) {
		if(strGrade == GetGrade(idx))	return idx;
		idx++;
	}
	return -1;
}
   
CString CSteelGrade::GetStringData(long idx, long ID)
{
	CString str = "";
	if(idx >= GetSize())	return str;

	switch(ID) {
	case 0:
		str = m_GradeArr[idx]->m_strGrade;					break;
	case 1:
		str.Format("%lf", m_GradeArr[idx]->m_weDB);			break;
	case 2:
		str.Format("%lf", m_GradeArr[idx]->m_weDBPt);		break;
	case 3:
		str.Format("%lf", m_GradeArr[idx]->m_weDBPf);		break;
	case 4:
		str.Format("%lf", m_GradeArr[idx]->m_weDBPr);		break;
	case 5:
		str.Format("%lf", m_GradeArr[idx]->m_weDL);			break;
	case 6:
		str.Format("%lf", m_GradeArr[idx]->m_weDLPm);		break;
	case 7:
		str.Format("%lf", m_GradeArr[idx]->m_weDLPs);		break;
	default:
		str = "Out of Range ID";
	}

	return str;
}

