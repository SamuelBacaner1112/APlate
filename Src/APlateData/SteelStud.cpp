// SteelStud.cpp: implementation of the CSteelStud class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelStud.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelStudData::CSteelStudData()
{
	m_strName		= _T("");
	m_dUWeight19	= 0;
	m_dUWeight22	= 0;
	m_dUWeight25	= 0;
}

CSteelStudData::~CSteelStudData()
{

}

void CSteelStudData::Serialize(CArchive& ar)
{
	long nFlag = 0;

	if(ar.IsStoring()) 
	{
		ar << nFlag;
		ar << m_strName;
		ar << m_dUWeight19;
		ar << m_dUWeight22;
		ar << m_dUWeight25;
	}
	else
	{
		ar >> nFlag;
		ar >> m_strName;
		ar >> m_dUWeight19;
		ar >> m_dUWeight22;
		ar >> m_dUWeight25;
	}
}

double CSteelStudData::GetStudUnitWeight(double dSize)
{
	// 기존에는 19, 22, 25mm 외의 지름도 입력을 받고있었다..
	if(dSize <= 19)	return m_dUWeight19;
	if(dSize <= 22)	return m_dUWeight22;
	return	m_dUWeight25;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelStud::CSteelStud()
{
	SetDataInit();
}

CSteelStud::~CSteelStud()
{
	RemoveAll();
}

void CSteelStud::RemoveAll()
{
	long size = m_StudArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_StudArr[n];
	m_StudArr.RemoveAll();
}

void CSteelStud::Serialize(CArchive& ar)
{
	long nSize = 0;
	long nFlag = 0;
	if(ar.IsStoring()) 
	{
		ar << nFlag;

		nSize = 	m_StudArr.GetSize();
		ar << nSize;
		for(long n = 0 ; n < nSize ; n++)
			m_StudArr[n]->Serialize(ar);
	}
	else 
	{
		ar >> nFlag;

		RemoveAll();
		ar >> nSize;
		for(long n = 0 ; n < nSize ; n++) 
		{
			CSteelStudData *pData = new CSteelStudData;
			pData->Serialize(ar);
			m_StudArr.Add(pData);
		}
	}
}

void CSteelStud::SetDataInit()
{
	RemoveAll();
	char* szBaseStr[] =
	{
		"80,	0.236, 	0.305, 	0.000",
		"90,	0.258, 	0.334, 	0.000", 
		"100,	0.281, 	0.364, 	0.489", 
		"110,	0.303, 	0.394, 	0.528", 
		"120,	0.327, 	0.427, 	0.567", 
		"130,	0.350, 	0.457, 	0.606", 
		"140,	0.372,	0.487, 	0.645", 
		"150,	0.394, 	0.517, 	0.684", 
		"160,	0.416, 	0.547, 	0.723", 
		"170,	0.438, 	0.577, 	0.762", 
		"180,	0.460, 	0.607, 	0.801", 
		"190,	0.482, 	0.637, 	0.840",
		"200,	0.504, 	0.667, 	0.879", 
		"210,	0.526, 	0.697, 	0.918", 
		"220,	0.548, 	0.727, 	0.957", 
		"230,	0.570, 	0.757, 	0.996", 
		"240,	0.592, 	0.787, 	1.035",
		"250,	0.614, 	0.817, 	1.074" 
	};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);

	CStringArray SArr;
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;

		// File에서 입력한 데이터가 적은 경우 처리(ASSERT 등 방지)
		CSteelStudData	*pData = new CSteelStudData;
		long nIdx = 0;
		pData->m_strName				= SArr[nIdx++];
		pData->m_dUWeight19				= atof(SArr[nIdx++]);
		pData->m_dUWeight22				= atof(SArr[nIdx++]);
		pData->m_dUWeight25				= atof(SArr[nIdx++]);
		m_StudArr.Add(pData);
	}
}

CSteelStudData* CSteelStud::GetStudData(CString strName)
{
	for(long nIdx=0; nIdx<m_StudArr.GetSize(); nIdx++)
	{
		CSteelStudData	*pData = m_StudArr.GetAt(nIdx);
		if(pData->m_strName == strName)
			return pData;		
	}

	return NULL;
}

double CSteelStud::GetStudUnitWeight(CString strName, double dSize)
{
	double	dLength	= atof(strName);

	for(long nIdx=0; nIdx<m_StudArr.GetSize(); nIdx++)
	{
		CSteelStudData	*pData = m_StudArr.GetAt(nIdx);
		if(dLength <= atof(pData->m_strName))
		{
			if(dSize==19)	return pData->m_dUWeight19;
			if(dSize==22)	return pData->m_dUWeight22;
			if(dSize==25)	return pData->m_dUWeight25;
		}
	}

	return 0;

//	CSteelStudData	*pData = GetStudData(strName);
//	return pData->GetStudUnitWeight(dSize);
}
