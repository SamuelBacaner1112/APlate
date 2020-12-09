// LinkToBx.cpp: implementation of the CPlateBxObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateBxObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CPlateBxObject, CObject)

CPlateBxObject::CPlateBxObject()
{
	m_pGir = NULL;
	m_psBx = NULL;
	m_psBxEnd = NULL;
	m_nSelfCount = 0;
	m_dwFlag = 0;
}

CPlateBxObject::~CPlateBxObject()
{
}

void CPlateBxObject::Serialize(CArchive& ar)
{

	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_dwFlag;
	}
	else
	{
		ar >> nFlag;
		ar >> m_dwFlag;
	}
	SetModify(FALSE);
}

CPlateBxObject& CPlateBxObject::operator=(const CPlateBxObject& obj)
{
	if( this == &obj) return *this;

	m_pGir = obj.m_pGir;
	m_psBx = obj.m_psBx;
	m_psBxEnd = obj.m_psBxEnd;
	m_dwFlag = obj.m_dwFlag;

	return *this;
}

CPlateBasicIndex* CPlateBxObject::GetBx() const
{
	return IsCreate() ? m_pGir->GetBx(m_psBx) : NULL;
}

CPlateBasicIndex* CPlateBxObject::GetBxEnd() const
{
	return IsCreate() ? m_pGir->GetBx(m_psBxEnd) : NULL;
}

void CPlateBxObject::SetPOS(CPlateBasicIndex *pBx)
{
	if(pBx && IsCreate())
	{
		for(POSITION pos = m_pGir->GetPOSHead(); pos; pos = m_pGir->GetPOSNext(pos))
			if(pBx == m_pGir->GetBx( pos ))
			{
				m_psBx = pos;
				break;
			}

	}
}

void CPlateBxObject::Create(CPlateGirderApp* pGir, CPlateBasicIndex *pBx)
{
	m_pGir = pGir;
	SetPOS(pBx);
}	

/*
// Make와 다른시기에 객체내용 setting함

						Make위치(수량변화위치)  객체내용채우는 위치(SetDataFilled()사용위치)
LK_DIAPHRAGM			다이아프램위치입력		- A,P,CU,CL,CX 타입 dlg
LK_CROSSBEAM			다이아프램위치입력		- 가로보상세 dlg
LK_STRINGER				다이아프램위치입력		- 세로보상세 dlg
LK_DANMYUN				수직보강재위치입력		- B,D,E,F,G 타입 dlg
LK_SPLICE				현장이음위치입력		- 현장이음상세 dlg
LK_WEBHSTIFF_CENTER		거의 모든 dlg			- 수평보강재 dlg
LK_WEBHSTIFF_LEFT
LK_WEBHSTIFF_RIGHT
LK_JACKUP_CENTER		객업텀dlg				- A,P타입 dlg
LK_JACKUP_LEFT
LK_JACKUP_RIGHT

// Make와 같은시기에 객체내용도 setting함
LK_FACTCHAIN_UPPER		- 공장이음 위치 dlg
LK_FACTCHAIN_LOWER
LK_FACTCHAIN_LEFT
LK_FACTCHAIN_RIGHT
LK_JRIB_UPPER			- 종리브 위치 dlg
LK_JRIB_LOWER


*/

void CPlateBxObject::SetDataFilled(BOOL bFill/* = TRUE*/)
{
	SetBitFlag32(m_dwFlag,BIT_LK_FILL,bFill);
}

BOOL CPlateBxObject::IsDataFilled() const 
{
	return GetBitFlag32(m_dwFlag,BIT_LK_FILL);
}

void CPlateBxObject::SetModify(BOOL bModify/* = TRUE*/)
{
	SetBitFlag32(m_dwFlag,BIT_LK_MODIFY,bModify);
}

BOOL CPlateBxObject::IsModify() const 
{
	return GetBitFlag32(m_dwFlag,BIT_LK_MODIFY);
}
void CPlateBxObject::SetJustModified(BOOL bModify/* = TRUE*/)
{
	SetBitFlag32(m_dwFlag,BIT_LK_JUSTMODIFIED,bModify);
}

BOOL CPlateBxObject::IsJustModified() const 
{
	return GetBitFlag32(m_dwFlag,BIT_LK_JUSTMODIFIED);
}



