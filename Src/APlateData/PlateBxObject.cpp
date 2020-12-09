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
// Make�� �ٸ��ñ⿡ ��ü���� setting��

						Make��ġ(������ȭ��ġ)  ��ü����ä��� ��ġ(SetDataFilled()�����ġ)
LK_DIAPHRAGM			���̾�������ġ�Է�		- A,P,CU,CL,CX Ÿ�� dlg
LK_CROSSBEAM			���̾�������ġ�Է�		- ���κ��� dlg
LK_STRINGER				���̾�������ġ�Է�		- ���κ��� dlg
LK_DANMYUN				������������ġ�Է�		- B,D,E,F,G Ÿ�� dlg
LK_SPLICE				����������ġ�Է�		- ���������� dlg
LK_WEBHSTIFF_CENTER		���� ��� dlg			- ���򺸰��� dlg
LK_WEBHSTIFF_LEFT
LK_WEBHSTIFF_RIGHT
LK_JACKUP_CENTER		������dlg				- A,PŸ�� dlg
LK_JACKUP_LEFT
LK_JACKUP_RIGHT

// Make�� �����ñ⿡ ��ü���뵵 setting��
LK_FACTCHAIN_UPPER		- �������� ��ġ dlg
LK_FACTCHAIN_LOWER
LK_FACTCHAIN_LEFT
LK_FACTCHAIN_RIGHT
LK_JRIB_UPPER			- ������ ��ġ dlg
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



