// GuardFence.cpp: implementation of the CGuardFence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuardFence.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardFence::CGuardFence()
{
	m_D1		=  0;
	m_W1		=  0;
	m_W2		=  0;
	m_W3		=  0;
	m_W4		=  0;
	m_W5		=  0;
	m_H1		=  0;
	m_H2		=  0;
	m_H3		=  0;
	m_nDirType	= -1;
}

CGuardFence::~CGuardFence()
{

}

void CGuardFence::Serialize(CArchive &ar)
{
	long nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_D1;
		ar << m_W1;
		ar << m_W2;
		ar << m_W3;
		ar << m_W4;
		ar << m_W5;
		ar << m_H1;
		ar << m_H2;
		ar << m_H3;
		ar << m_nDirType;
	}
	else
	{
		ar >> nFlag;
		ar >> m_D1;
		ar >> m_W1;
		ar >> m_W2;
		ar >> m_W3;
		ar >> m_W4;
		ar >> m_W5;
		ar >> m_H1;
		ar >> m_H2;
		ar >> m_H3;
		ar >> m_nDirType;
	}
}

CGuardFence& CGuardFence::operator = (const CGuardFence& obj)
{
	m_D1		= obj.m_D1;
	m_W1		= obj.m_W1;
	m_W2		= obj.m_W2;
	m_W3		= obj.m_W3;
	m_W4		= obj.m_W4;
	m_W5		= obj.m_W5;
	m_H1		= obj.m_H1;
	m_H2		= obj.m_H2;
	m_H3		= obj.m_H3;
	m_nDirType	= obj.m_nDirType;

	return *this;
}

inline long CGuardFence::GetPosition()
{
	if(m_nDirType == -1) return LEFT;
	if(m_nDirType ==  1) return RIGHT;

	return CENTER;
}

inline double CGuardFence::GetHeight() const
{
	return m_H1+m_H2+m_H3;
}

inline double CGuardFence::GetWidth() const
{
	return m_W1+m_W2+m_W3+m_W4+m_W5;
}
