// SupportRFBarSet.cpp: implementation of the CSupportRFBarSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SupportRFBarSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSupportRFBarSet::CSupportRFBarSet()
{
	m_vecSupportRFBar.clear();			
	m_dDiaSptRFBar = 0;				
	m_dLenSptRFBar = 0;				
	m_dArcRadiusSptRFBar = 0;		
	m_dArcAngSptRFBar = 0;	
	m_dFy = 0;
}

CSupportRFBarSet::~CSupportRFBarSet()
{

}



void CSupportRFBarSet::Serialize(CArchive &ar)
{
	long nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		
		ar << GetSize();
		for(long i = 0; i < GetSize(); i++)
			m_vecSupportRFBar[i].Serialize(ar);	//�������� ����ö��

		ar << m_dDiaSptRFBar;					//�������� ����ö�� ����
		ar << m_dLenSptRFBar;					//�������� ����ö�� ��ü���� 
		ar << m_dArcRadiusSptRFBar;				//�������� ����ö�� Arc Radius
		ar << m_dArcAngSptRFBar;				//�������� ����ö�� Arc Angle
		ar << m_dFy;							//ö�� �׺� ���� 
	}
	else
	{
		long nSize = 0;
		ar >> nFlag;
		
		ar >> nSize;
		for(long i = 0; i < nSize; i++)
		{
			CSupportRFBar bar;
			bar.Serialize(ar);
			m_vecSupportRFBar.push_back(bar);
		}
		ar >> m_dDiaSptRFBar;					//�������� ����ö�� ����
		ar >> m_dLenSptRFBar;					//�������� ����ö�� ��ü���� 
		ar >> m_dArcRadiusSptRFBar;				//�������� ����ö�� Arc Radius
		ar >> m_dArcAngSptRFBar;				//�������� ����ö�� Arc Angle
		ar >> m_dFy;
	}
}

void CSupportRFBarSet::AddRFBar(CSupportRFBar rfbar)
{
	m_vecSupportRFBar.push_back(rfbar);
}


BOOL CSupportRFBarSet::RemoveRFBar(int nIdx)
{
	int nSize = m_vecSupportRFBar.size();
	if(nIdx < 0 || nIdx >= nSize)
		return FALSE;

	// 2015. 03. 18 ������ :: Sonar Qube Major Issue ó���� ���ؼ� for�� ���ش�. (��� for�� ��� �ȴ�.)
	m_vecSupportRFBar.erase(m_vecSupportRFBar.begin() + nIdx);

	/*
	// ���� �ҽ��ڵ�
	int nCnt = 0;
	for(iterPos = m_vecSupportRFBar.begin(); iterPos != m_vecSupportRFBar.end(); ++iterPos)
	{
		nCnt++;
		if(nCnt == nIdx)
			m_vecSupportRFBar.erase(iterPos);
	}
	*/

	return TRUE;
}

void CSupportRFBarSet::SetDataDefault(BOOL bRecommend)
{
	if(bRecommend == TRUE)
	{
		SetDiaRFBar(16);
		SetLenRFBar(2000);
		SetArcAngleRFBar(50);
		SetArcRadiusRFBar(100);
	}
}

void CSupportRFBarSet::DrawRebar(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	int nSize = GetSize();
	for(int i = 0; i < nSize; i++)
		m_vecSupportRFBar[i].DrawRebar(&Dom);

	*pDom << Dom;
}


int CSupportRFBarSet::GetSize()
{
	int nSize = 0;
	nSize = m_vecSupportRFBar.size();
	return nSize;
}


void CSupportRFBarSet::DrawStud(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	//�������� 
	int nSize = GetSize();
	for(int i = 0; i < nSize; i++)
		m_vecSupportRFBar[i].DrawStud(&Dom);

	*pDom << Dom;
}


void CSupportRFBarSet::DimRebar(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	CDPointArray ptArrDim;
	CDPointArray ptArrLast;
	CDPoint ptStt, ptEnd;
	double dDis = 0;

	int nSize = GetSize();
	if(nSize <= 1)	return;

	ptStt = m_vecSupportRFBar[0].GetCalcStudCenter();
	ptEnd = m_vecSupportRFBar[1].GetCalcStudCenter();
	dDis = ~(ptStt - ptEnd);

	ptArrLast = m_vecSupportRFBar[nSize - 1].GetPosRebar(TRUE);
	int nSizeEx = ptArrLast.GetSize();
	CDPoint ptFirst = ptArrLast.GetAt(0);
	CDPoint ptLast = ptArrLast.GetAt(nSizeEx - 1);

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(ptStt.x, ptLast.y, 0);
	CString str;
	double dLen = dDis*(nSize - 1);
	str.Format("%d@%0.0f=%s", nSize-1, dDis, COMMA(dLen));
	Dom.DimLineTo(dLen, 0, str);

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(ptFirst.x, ptFirst.y, 0);
	dLen = ~(ptFirst - ptLast);
	str.Format("%s", COMMA(dLen));
	Dom.DimLineTo(dLen, 0, str);

	*pDom << Dom;
}

void CSupportRFBarSet::SetDiaRFBar(double dDia)
{
	m_dDiaSptRFBar = dDia;

	int nSize = GetSize();
	for(int i = 0; i < nSize; i++)
		m_vecSupportRFBar[i].SetDia(dDia);
}

void CSupportRFBarSet::SetLenRFBar(double dLen)
{
	m_dLenSptRFBar = dLen;

	int nSize = GetSize();
	for(int i = 0; i < nSize; i++)
		m_vecSupportRFBar[i].SetTotalLength(dLen);
}

void CSupportRFBarSet::RemoveAll()
{
	m_vecSupportRFBar.clear();
	m_dDiaSptRFBar = 0;				
	m_dLenSptRFBar = 0;				
	m_dArcRadiusSptRFBar = 0;		
	m_dArcAngSptRFBar = 0;	
	m_dFy = 0;
}

void CSupportRFBarSet::SetArcRadiusRFBar(double dRadius)
{
	m_dArcRadiusSptRFBar = dRadius;

	int nSize = GetSize();
	for(int i = 0; i < nSize; i++)
		m_vecSupportRFBar[i].SetArcRadius(dRadius);
}


void CSupportRFBarSet::SetArcAngleRFBar(double dAngle)
{
	m_dArcAngSptRFBar = dAngle;

	int nSize = GetSize();
	for(int i = 0; i < nSize; i++)
		m_vecSupportRFBar[i].SetArcAngle(dAngle);
}


void CSupportRFBarSet::RemoveRebarPos()
{
	m_vecSupportRFBar.clear();
}


CSupportRFBar* CSupportRFBarSet::GetAt(int nIdx)
{
	CSupportRFBar* rfBar = NULL;
	int nSize = GetSize();
	if(nIdx < 0 || nIdx >= nSize)
		return rfBar;

	rfBar = &m_vecSupportRFBar[nIdx];
	return rfBar;

}

CString CSupportRFBarSet::GetTextRebarDia()
{
	CString strDia;

	strDia.Format("%c%0.0f", GetCharRebarMark(m_dFy), m_dDiaSptRFBar);

	return strDia;
}
