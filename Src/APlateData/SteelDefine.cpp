// SteelDefine.cpp: implementation of the CSteelDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define DATASU	3

CSteelDefine::CSteelDefine()
{
	SetDataInit();
}

CSteelDefine::~CSteelDefine()
{
	RemoveAll();
}

void CSteelDefine::RemoveAll()
{
	long size = m_SteelDefArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_SteelDefArr[n];
	m_SteelDefArr.RemoveAll();
}

void CSteelDefine::Serialize(CArchive& ar)
{
	long size  = 0;
	long nFlag = 0;

	if(ar.IsStoring()) 
	{
		ar << nFlag;
		size = 	m_SteelDefArr.GetSize();
		ar << size;
		for(long n = 0 ; n < size ; n++)
			m_SteelDefArr[n]->Serialize(ar);
	}
	else
	{
		ar >> nFlag;

		CStringArray strArr;

		for(long nIdx=0; nIdx<m_SteelDefArr.GetSize(); nIdx++)
			strArr.Add(m_SteelDefArr[nIdx]->m_strVarName);

		RemoveAll();
		ar >> size;
		for(long n = 0 ; n < size ; n++)
		{
			CSteelDefineData *pData = new CSteelDefineData;
			
			pData->Serialize(ar);

			// Serialize()���� m_strVarName�� �������� ����
			if(n < strArr.GetSize())
				pData->m_strVarName = strArr.GetAt(n);

			m_SteelDefArr.Add(pData);
		}
	}
}

void CSteelDefine::SetDataInit()
{
	char* szBaseStr[] = {
		"�ֺ���,    ������ ����,    SM490B",
		"�κ���,    ������ ����,    SM400B",
		"��Ʈ  ,    ��Ʈ       ,    F10T   ",
		"����  ,    ����       ,    SD30  ",
		"�ޱ�  ,    ����       ,    SD30  ",
		"��Ÿ  ,    ALL        ,    SM400B",
		"����  ,    ALL        ,    SM400B",
		};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;

		// File���� �Է��� �����Ͱ� ���� ��� ó��
		while( SArr.GetSize() < DATASU ) SArr.Add(_T(""));

		CSteelDefineData *pData = new CSteelDefineData;

		pData->m_strClass	=	SArr[0];
		pData->m_strVarName	=	SArr[1];
		pData->m_strSymbol	=	SArr[2];

		m_SteelDefArr.Add(pData);
	}
}

CString CSteelDefine::GetStringData(long idx, STLDEF_DATA_ID ID)
{
	CString str = "";
	if(idx >= GetSize())	return str;

	switch(ID) {
	case BDISD_CLASS:
		return m_SteelDefArr[idx]->m_strClass;
	case BDISD_VARNAME:
		return m_SteelDefArr[idx]->m_strVarName;
	case BDISD_SYMBOL:
		return m_SteelDefArr[idx]->m_strSymbol;
	default:
		str = "Out of Range ID";
	}
	return str;
}
