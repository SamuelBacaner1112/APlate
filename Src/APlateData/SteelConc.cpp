// SteelConc.cpp: implementation of the CSteelConc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelConc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelConc::CSteelConc()
{
	SetDataInit();
}

CSteelConc::~CSteelConc()
{
	RemoveAll();
}

void CSteelConc::RemoveAll()
{
	long size = m_ConArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_ConArr[n];
	m_ConArr.RemoveAll();
}

void CSteelConc::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		AhTPASerial(ar, &m_ConArr, (CSteelConcData *)0);
	}
	else
	{
		ar >> nFlag;
		AhTPASerial(ar, &m_ConArr, (CSteelConcData *)0);
	}
}

#define DATASU	12
void CSteelConc::SetDataInit()
{

	char* szBaseStr[] = {
		"S21,  21.0,  1.00,  0.000012,  22000,  0.0000250,  10.00,  0.000270,  2.0,  4.0,  0.0,  250.0",
		"S24,  24.0,  1.00,  0.000012,  23500,  0.0000250,  10.00,  0.000270,  2.0,  4.0,  0.0,  250.0",
		"S27,  27.0,  1.00,  0.000012,  25000,  0.0000250,  10.00,  0.000270,  2.0,  4.0,  0.0,  250.0",
		"S30,  30.0,  1.00,  0.000012,  26000,  0.0000250,  10.00,  0.000270,  2.0,  4.0,  0.0,  250.0",
		"S40,  40.0,  1.00,  0.000012,  30000,  0.0000250,  10.00,  0.000270,  2.0,  4.0,  0.0,  250.0",
		"S50,  50.0,  1.00,  0.000012,  34000,  0.0000250,  10.00,  0.000270,  2.0,  4.0,  0.0,  250.0",
	};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);	

	RemoveAll();
	
	CStringArray SArr;
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;

		// File에서 입력한 데이터가 적은 경우 처리
		while( SArr.GetSize() < DATASU ) SArr.Add(_T(""));

		CSteelConcData* pData = new CSteelConcData;

		pData->m_strSymbol		=	SArr[0];
		pData->m_BasicStrength  =	atof(SArr[1]);
		pData->m_SafetyRate		=	atof(SArr[2]);
		pData->m_Alpha			=	atof(SArr[3]);
		pData->m_ElasticModulus	=	atof(SArr[4]);
		pData->m_UnitWeight		=	atof(SArr[5]);
		pData->m_Temperature	=	atof(SArr[6]);
		pData->m_Epsilon		=	atof(SArr[7]);
		pData->m_Creep1			=	atof(SArr[8]);
		pData->m_Creep2			=	atof(SArr[9]);
		pData->m_BendingStress	=	atof(SArr[10]);
		pData->m_ShearStress	=	atof(SArr[11]);

		m_ConArr.Add(pData);
	}
}

long	CSteelConc::GetIndex(CString strSymbol)
{
	long idx, size;

	idx = 0;
	size = GetSize();
	while(idx < size) {
		if(strSymbol == GetSymbol(idx))	return idx;
		idx++;
	}
	return -1;
}

CString CSteelConc::GetStringData(long idx, long ID)
{
	CString str = "";
	if(idx >= GetSize())	return str;

	switch(ID) {
	case 0:
		str = m_ConArr[idx]->m_strSymbol;					break;
	case 1:
		str.Format("%lf", m_ConArr[idx]->m_BasicStrength);	break;
	case 2:
		str.Format("%lf", m_ConArr[idx]->m_SafetyRate);		break;
	case 3:
		str.Format("%lf", m_ConArr[idx]->m_Alpha);			break;
	case 4:
		str.Format("%lf", m_ConArr[idx]->m_ElasticModulus);	break;
	case 5:
		str.Format("%lf", m_ConArr[idx]->m_UnitWeight);		break;
	case 6:
		str.Format("%lf", m_ConArr[idx]->m_Temperature);	break;
	case 7:
		str.Format("%lf", m_ConArr[idx]->m_Epsilon);		break;
	case 8:
		str.Format("%lf", m_ConArr[idx]->m_Creep1);			break;
	case 9:
		str.Format("%lf", m_ConArr[idx]->m_Creep2);			break;
	case 10:
		str.Format("%lf", m_ConArr[idx]->m_BendingStress);	break;
	case 11:
		str.Format("%lf", m_ConArr[idx]->m_ShearStress);	break;
	default: str = "Out of Range ID";
	}
	return str;
}


