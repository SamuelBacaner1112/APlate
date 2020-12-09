// VStiff.cpp: implementation of the CVStiff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "VStiff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// For 지점 보강재
CVStiff::CVStiff() : m_pSection(NULL)
{
	m_dH[0][0] = 0;	//LU
	m_dH[0][1] = 0;	//LD
	m_dH[1][0] = 0;	//RU
	m_dH[1][1] = 0;	//RD

	for(long nCol=0; nCol<COUNT; nCol++)
	{
		m_dThick[nCol] = 12;
		m_dWidth[nCol] = 150;
		m_dGap[nCol] = 20;
	}
	m_cCarryUpper = FALSE;
	m_cCarryLower = FALSE;
}	
CVStiff::CVStiff(CSection *pSection) : m_pSection(pSection)
{
	m_dH[0][0] = 0;	//LU
	m_dH[0][1] = 0;	//LD
	m_dH[1][0] = 0;	//RU
	m_dH[1][1] = 0;	//RD

	for(long nCol=0; nCol<COUNT; nCol++)
	{
		m_dThick[nCol] = 12;
		m_dWidth[nCol] = 150;
		m_dGap[nCol] = 20;
	}
}

CPlateBasicIndex *CVStiff::GetBx()
{ 
	return (CPlateBasicIndex *)m_pSection->GetBx(); 
}

void CVStiff::Serialize(CArchive &ar)
{
	long nFlag  = 1;	// 071224 lina
	long nCount = COUNT;
	if(ar.IsStoring())
	{
		m_nFlag = nFlag;
		ar << nFlag;
		ar << nCount;
		for(long nRow=0; nRow<COUNT; nRow++)
			for(long nCol=0; nCol<COUNT; nCol++)
				ar << m_dH[nRow][nCol];

		for(long nCol=0; nCol<COUNT; nCol++)
		{
			ar << m_dThick[nCol];
			ar << m_dWidth[nCol];
			ar << m_dGap[nCol];
		}
		ar << m_cCarryUpper;
		ar << m_cCarryLower;

	}
	else
	{
		ar >> nFlag;
		ar >> nCount;

		if(nFlag>0)
		{
			ar >> m_dH[LEFT][UP];
			ar >> m_dH[LEFT][DOWN];
			ar >> m_dH[RIGHT][UP];
			ar >> m_dH[RIGHT][DOWN];
			m_nFlag = 1;
		}
		else
		{
			double dTemp=0;
			ar >> m_dLeft;
			ar >> m_dRight;
			ar >> dTemp;
			ar >> dTemp;
			m_nFlag = 0;

			// Flag 1 이전 파일이 TU거더일리 없으므로 상단 이격거리는 0으로 처리
			// 기존에서 하단 이격거리를 제대로 저장하지 않음.
			// 전체 Serialize 완료 후 CPlateGirder에서 재설정함.

			m_dH[LEFT][UP] = 0;
			m_dH[LEFT][DOWN] = 0;
			m_dH[RIGHT][UP] = 0;
			m_dH[RIGHT][DOWN] = 0;
		}

		for(long nCol=0; nCol<COUNT; nCol++)
		{
			ar >> m_dThick[nCol];
			ar >> m_dWidth[nCol];
			ar >> m_dGap[nCol];
		}

		ar >> m_cCarryUpper;
		ar >> m_cCarryLower;
	}
}

CVStiff& CVStiff::operator=(const CVStiff& Obj)
{
	for(long n=0; n<COUNT; n++)
	{
		m_dThick[n] = Obj.m_dThick[n];
		m_dWidth[n] = Obj.m_dWidth[n];
		for(long nn=0; nn<COUNT; nn++)
			m_dH[n][nn] = Obj.m_dH[n][nn];
		m_dGap[n] = Obj.m_dGap[n];
	}

	m_cCarryUpper = Obj.m_cCarryUpper;
	m_cCarryLower = Obj.m_cCarryLower;

	return *this;
}

double CVStiff::GetVStiffHeight(BOOL bLeft, double dHGir)
{
	if(dHGir<0)
		dHGir = m_pSection->GetGirder()->GetHeightGirderByStaAng(GetBx()->GetStation());
	if(bLeft)
		return max(0, dHGir - m_dH[LEFT][UP] - m_dH[LEFT][DOWN]);
	else
		return max(0, dHGir - m_dH[RIGHT][UP] - m_dH[RIGHT][DOWN]);
}
