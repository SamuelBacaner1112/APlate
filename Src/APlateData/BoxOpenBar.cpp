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


//�밢�� ö���� ��ġ ���� ��ȯ 
CDPointArray CBoxOpenBar::GetPosDiagonalBar(int nType)
{
	double dSpace = 0;
	if(nType == 0 || nType == 1)  //�»�, ��� 
		dSpace = -m_dSpaceRebar;
	else if(nType == 2 || nType == 3) //����, ����
		dSpace = m_dSpaceRebar;

	CDPointArray ptArrLine;
	//��� ö�� ��ġ 
	CDPoint ptStt, ptEnd;
	GetPosFirstDiagonalBar(ptStt, ptEnd, nType); //��� 
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


//������ ö���� ��ġ ���� ��ȯ
CDPointArray CBoxOpenBar::GetPosStraightBar(int nType)
{
	double dSpace = 0;
	if(nType == 0 || nType == 2)  //��� ����   
		dSpace = -m_dSpaceRebar;
	else if(nType == 1 || nType == 3) //�Ϻ�, ���� 
		dSpace = m_dSpaceRebar;

	CDPointArray ptArrLine;
	//��� ö�� ��ġ 
	CDPoint ptStt, ptEnd;
	GetPosFirstStraightBar(ptStt, ptEnd, nType); //��� 
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

//����ó�� ������ ö���� ��ġ ���� ��ȯ
void CBoxOpenBar::GetPosFirstStraightBar(CDPoint& ptStt, CDPoint& ptEnd, int nType)
{
	double dHeight = 0, dWidth = 0;
	if(m_nTypeOpen == 0)  //�����ΰ� ���� Ÿ�� 
	{
		dHeight = m_dRadiusOpen;
		dWidth = m_dRadiusOpen;
	}
	else
	{
		dHeight = m_dLengthOpen/2;
		dWidth = m_dWidthOpen/2;
	}


	if(nType == 0)  //��� 
	{
		ptStt.x = m_ptCen.x - m_dLengthRebar/2;
		ptStt.y = m_ptCen.y + dHeight + m_dCover;
		ptEnd.x = m_ptCen.x + m_dLengthRebar/2;
		ptEnd.y = m_ptCen.y + dHeight + m_dCover;
	}
	else if(nType == 1)  //�Ϻ� 
	{
		ptStt.x = m_ptCen.x - m_dLengthRebar/2;
		ptStt.y = m_ptCen.y - dHeight - m_dCover;
		ptEnd.x = m_ptCen.x + m_dLengthRebar/2;
		ptEnd.y = m_ptCen.y - dHeight - m_dCover;
	}
	else if(nType == 2)  //���� 
	{
		ptStt.x = m_ptCen.x - dWidth - m_dCover;
		ptStt.y = m_ptCen.y - m_dLengthRebar/2;
		ptEnd.x = m_ptCen.x - dWidth - m_dCover;
		ptEnd.y = m_ptCen.y + m_dLengthRebar/2;
	}
	else if(nType == 3)  //���� 
	{
		ptStt.x = m_ptCen.x + dWidth + m_dCover;
		ptStt.y = m_ptCen.y - m_dLengthRebar/2;
		ptEnd.x = m_ptCen.x + dWidth + m_dCover;
		ptEnd.y = m_ptCen.y + m_dLengthRebar/2;
	}
}


//����ó�� �밢�� ö���� ��ġ ���� ��ȯ 
void CBoxOpenBar::GetPosFirstDiagonalBar(CDPoint& ptStt, CDPoint& ptEnd, int nType)
{
	CDPoint ptDot[4];
	CDPoint ptAng;
	double dAng = 0, dSpace = 0;
	double dHeight = 0, dWidth = 0;

	if(m_nTypeOpen == 0)  //�����ΰ� ���� Ÿ�� 
	{
		dHeight = m_dRadiusOpen;
		dWidth = m_dRadiusOpen;
	}
	else
	{
		dHeight = m_dLengthOpen/2;
		dWidth = m_dWidthOpen/2;
	}

	//�»� 
	ptDot[0].x = m_ptCen.x - dWidth - m_dCover;
	ptDot[0].y = m_ptCen.y + dHeight + m_dCover;

	//��� 
	ptDot[1].x = m_ptCen.x + dWidth + m_dCover;
	ptDot[1].y = m_ptCen.y + dHeight + m_dCover;

	//���� 
	ptDot[2].x = m_ptCen.x - dWidth - m_dCover;
	ptDot[2].y = m_ptCen.y - dHeight - m_dCover;

	//���� 
	ptDot[3].x = m_ptCen.x + dWidth + m_dCover;
	ptDot[3].y = m_ptCen.y - dHeight - m_dCover;

	if(nType == 0 || nType == 3) //�»� or ���� 
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
	else if(nType == 1 || nType == 2)  //��� or ���� 
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
		ar << m_dCover;		//������ ö���� �Ǻ��β�
	}
	else
	{
		ar >> nFlag;
		ar >> m_dCover;		//������ ö���� �Ǻ��β�
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
		m_strOpenType = "������";	
		m_nCntRebar = 3;	
		m_dSpaceRebar = 100;	
		m_dDiaRebar = 16;	
		m_dLengthRebar = 1500;	
		m_dCover = 100;
		m_dRadiusOpen = 100;
	}	
}	



