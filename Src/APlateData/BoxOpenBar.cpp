// BoxOpenBar.cpp: implementation of the CBoxOpenBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoxOpenBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoxOpenBar::CBoxOpenBar()
{

}

CBoxOpenBar::~CBoxOpenBar()
{

}


//대각선 철근의 위치 정보 반환 
CDPointArray CBoxOpenBar::GetPosDiagonalBar(int nType)
{
	double dSpace = 0;
	if(nType == 0 || nType == 1)  //좌상, 우상 
		dSpace = -m_dSpaceRebar;
	else if(nType == 2 || nType == 3) //좌하, 우하
		dSpace = m_dSpaceRebar;

	CDPointArray ptArrLine;
	//상부 철근 위치 
	CDPoint ptStt, ptEnd;
	GetPosFirstDiagonalBar(ptStt, ptEnd, nType); //상부 
	ptArrLine.Add(ptStt);
	ptArrLine.Add(ptEnd);
	//OffSet
	for(int i = 0; i < m_nCntRebar - 1; i++)
	{
		Offset(ptStt, ptEnd, dSpace);
		ptArrLine.Add(ptStt);
		ptArrLine.Add(ptEnd);
	}

	return ptArrLine;	
}


//직선형 철근의 위치 정보 반환
CDPointArray CBoxOpenBar::GetPosStraightBar(int nType)
{
	double dSpace = 0;
	if(nType == 0 || nType == 2)  //상부 좌측   
		dSpace = -m_dSpaceRebar;
	else if(nType == 1 || nType == 3) //하부, 우측 
		dSpace = m_dSpaceRebar;

	CDPointArray ptArrLine;
	//상부 철근 위치 
	CDPoint ptStt, ptEnd;
	GetPosFirstStraightBar(ptStt, ptEnd, nType); //상부 
	ptArrLine.Add(ptStt);
	ptArrLine.Add(ptEnd);
	//OffSet
	for(int i = 0; i < m_nCntRebar - 1; i++)
	{
		Offset(ptStt, ptEnd, dSpace);
		ptArrLine.Add(ptStt);
		ptArrLine.Add(ptEnd);
	}

	return ptArrLine;
}

//제일처음 직선형 철근의 위치 정보 반환
void CBoxOpenBar::GetPosFirstStraightBar(CDPoint& ptStt, CDPoint& ptEnd, int nType)
{
	double dHeight = 0, dWidth = 0;
	if(m_nTypeOpen == 0)  //개구부가 원형 타입 
	{
		dHeight = m_dRadiusOpen;
		dWidth = m_dRadiusOpen;
	}
	else
	{
		dHeight = m_dLengthOpen/2;
		dWidth = m_dWidthOpen/2;
	}


	if(nType == 0)  //상부 
	{
		ptStt.x = m_ptCen.x - m_dLengthRebar/2;
		ptStt.y = m_ptCen.y + dHeight + m_dCover;
		ptEnd.x = m_ptCen.x + m_dLengthRebar/2;
		ptEnd.y = m_ptCen.y + dHeight + m_dCover;
	}
	else if(nType == 1)  //하부 
	{
		ptStt.x = m_ptCen.x - m_dLengthRebar/2;
		ptStt.y = m_ptCen.y - dHeight - m_dCover;
		ptEnd.x = m_ptCen.x + m_dLengthRebar/2;
		ptEnd.y = m_ptCen.y - dHeight - m_dCover;
	}
	else if(nType == 2)  //좌측 
	{
		ptStt.x = m_ptCen.x - dWidth - m_dCover;
		ptStt.y = m_ptCen.y - m_dLengthRebar/2;
		ptEnd.x = m_ptCen.x - dWidth - m_dCover;
		ptEnd.y = m_ptCen.y + m_dLengthRebar/2;
	}
	else if(nType == 3)  //우측 
	{
		ptStt.x = m_ptCen.x + dWidth + m_dCover;
		ptStt.y = m_ptCen.y - m_dLengthRebar/2;
		ptEnd.x = m_ptCen.x + dWidth + m_dCover;
		ptEnd.y = m_ptCen.y + m_dLengthRebar/2;
	}
}


//제일처음 대각선 철근의 위치 정보 반환 
void CBoxOpenBar::GetPosFirstDiagonalBar(CDPoint& ptStt, CDPoint& ptEnd, int nType)
{
	CDPoint ptDot[4];
	CDPoint ptAng;
	double dAng = 0, dSpace = 0;
	double dHeight = 0, dWidth = 0;

	if(m_nTypeOpen == 0)  //개구부가 원형 타입 
	{
		dHeight = m_dRadiusOpen;
		dWidth = m_dRadiusOpen;
	}
	else
	{
		dHeight = m_dLengthOpen/2;
		dWidth = m_dWidthOpen/2;
	}

	//좌상 
	ptDot[0].x = m_ptCen.x - dWidth - m_dCover;
	ptDot[0].y = m_ptCen.y + dHeight + m_dCover;

	//우상 
	ptDot[1].x = m_ptCen.x + dWidth + m_dCover;
	ptDot[1].y = m_ptCen.y + dHeight + m_dCover;

	//좌하 
	ptDot[2].x = m_ptCen.x - dWidth - m_dCover;
	ptDot[2].y = m_ptCen.y - dHeight - m_dCover;

	//우하 
	ptDot[3].x = m_ptCen.x + dWidth + m_dCover;
	ptDot[3].y = m_ptCen.y - dHeight - m_dCover;

	if(nType == 0 || nType == 3) //좌상 or 우하 
	{
		ptAng = (ptDot[1] - ptDot[2]).Unit();
		dAng = 180 + ptAng.GetAngleDegree();
		ptStt = m_ptCen + ToDPointFrDegree(dAng)*(m_dLengthRebar/2);
		dAng = ptAng.GetAngleDegree();
		ptEnd = m_ptCen + ToDPointFrDegree(dAng)*(m_dLengthRebar/2);
		dSpace = ~(m_ptCen - ptDot[0]);
		//OffSet
		if(nType == 0)
			Offset(ptStt, ptEnd, -dSpace);
		else if(nType == 3)
			Offset(ptStt, ptEnd, dSpace);
	}
	else if(nType == 1 || nType == 2)  //우상 or 좌하 
	{
		ptAng = (ptDot[0] - ptDot[3]).Unit();
		dAng = ptAng.GetAngleDegree();
		ptStt = m_ptCen + ToDPointFrDegree(dAng)*(m_dLengthRebar/2);
		dAng = 180 + ptAng.GetAngleDegree();
		ptEnd = m_ptCen + ToDPointFrDegree(dAng)*(m_dLengthRebar/2);
		dSpace = ~(m_ptCen - ptDot[1]);
		//OffSet
		if(nType == 1)
			Offset(ptStt, ptEnd, -dSpace);
		else if(nType == 2)
			Offset(ptStt, ptEnd, dSpace);
	}
}


void CBoxOpenBar::Serialize(CArchive &ar)
{
	long nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_dCover;		//개구부 철근의 피복두께
	}
	else
	{
		ar >> nFlag;
		ar >> m_dCover;		//개구부 철근의 피복두께
	}

	COpenBar::Serialize(ar);

}

void CBoxOpenBar::SetDataDefault(BOOL bRecommend)
{
	if(bRecommend)
	{
		m_ptCen.x = 0;
		m_ptCen.y = 0;		
		m_nTypeOpen = 1;			
		m_dRadiusOpen = 0;			
		m_dWidthOpen = 200;			
		m_dLengthOpen = 200;		
		m_strOpenType = "집수구";	
		m_nCntRebar = 3;	
		m_dSpaceRebar = 100;	
		m_dDiaRebar = 16;	
		m_dLengthRebar = 1500;	
		m_dCover = 100;
		m_dRadiusOpen = 100;
	}	
}	



