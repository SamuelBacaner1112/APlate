// NewBujeName.cpp: implementation of the CNewBujeName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewBujeName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNewBujeNameEl::CNewBujeNameEl()
{

}

CNewBujeNameEl::~CNewBujeNameEl()
{

}

// return value : Add=TRUE;
BOOL CNewBujeNameEl::AddString(const CString& sEle)
{
	for(long n = 0; n < m_EleArr.GetSize(); n++)
		if(sEle==m_EleArr[n]) return FALSE;

	m_EleArr.Add(sEle);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNewBujeName::CNewBujeName()
{

}

CNewBujeName::~CNewBujeName()
{
}

void CNewBujeName::AddEle(const CString& sSec,const CString& sEle)
{
	for(long n = 0; n < m_NBNameArr.GetSize(); n++)
	{
		CNewBujeNameEl * pEl = (CNewBujeNameEl *)m_NBNameArr.GetAt(n);
		if(pEl->m_sSecName == sSec)
		{
			pEl->AddString(sEle);
			return;
		}
	}

	CNewBujeNameEl * pEl = new CNewBujeNameEl;
	pEl->m_sSecName = sSec;
	pEl->AddString(sEle);
	m_NBNameArr.Add(pEl);
}


void CNewBujeName::ClearAll()
{
	for(long n = 0; n < m_NBNameArr.GetSize(); n++)
		delete (CNewBujeNameEl*)m_NBNameArr.GetAt(n);
	m_NBNameArr.RemoveAll();
}

BOOL CNewBujeName::GetEleName(CStringArray &Arr,const CString& sSec) const
{
	for(long n = 0; n < m_NBNameArr.GetSize(); n++)
	{
		CNewBujeNameEl * pEl = (CNewBujeNameEl *)m_NBNameArr.GetAt(n);
		if(pEl->m_sSecName == sSec)
		{
			Arr.RemoveAll();
			for(long i = 0; i < pEl->m_EleArr.GetSize(); i++)
				Arr.Add(pEl->m_EleArr[i]);
			return TRUE;
		}
	}
	return FALSE;
}
