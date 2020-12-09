// OpenBar.cpp: implementation of the COpenBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenBar::COpenBar()
{
	m_nCntRebar = 0;	
	m_dSpaceRebar = 0;	
	m_dDiaRebar = 0;	
	m_dLengthRebar = 0;	
	m_nTypeOpen = 0;	
	m_dRadiusOpen = 0;	
	m_dWidthOpen = 0;	
	m_dLengthOpen = 0;	
	m_dFy = 0;
}


COpenBar::COpenBar(const COpenBar& obj)
{
	m_ptCen = obj.m_ptCen;		//개구부 중심위치 
	m_nTypeOpen = obj.m_nTypeOpen;	//개구부 타입(0 : 원형, 1:박스형)
	m_dRadiusOpen = obj.m_dRadiusOpen;	//원형타입일때 개구부 반지름 
	m_dWidthOpen = obj.m_dWidthOpen;	//박스타입일때 개구부 가로길이
	m_dLengthOpen = obj.m_dLengthOpen;	//박스타입일때 개구부 세로길이 
	m_strOpenType = m_strOpenType;	//개구부 종류 이름 

	m_nCntRebar = m_nCntRebar;	//한 방향의 철근 개수
	m_dSpaceRebar = m_dSpaceRebar;	//철근간격 
	m_dDiaRebar = m_dDiaRebar;	//철근직경
	m_dLengthRebar = m_dLengthRebar;	//철근전체 길이
	m_dFy = obj.m_dFy;
}


COpenBar::~COpenBar()
{

}



//개구부를 그린다.
void COpenBar::DrawOpen(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	if(m_nTypeOpen == 0)  //원형타입 
	{
		Dom.Circle(m_ptCen, m_dRadiusOpen);		
	}
	else if(m_nTypeOpen == 1)  //박스타입 
	{
		CDPoint ptLT(m_ptCen.x - m_dWidthOpen/2, m_ptCen.y + m_dLengthOpen/2);
		CDPoint ptRB(m_ptCen.x + m_dWidthOpen/2, m_ptCen.y - m_dLengthOpen/2);
		Dom.Rectangle(ptLT, ptRB);

		// X 표시 
		Dom.LineTo(ptLT, ptRB);
		Dom.LineTo(ptLT.x + m_dWidthOpen, ptLT.y, ptRB.x - m_dWidthOpen, ptRB.y);
	}

	//문자열 출력
	if(m_strOpenType.GetLength() != 0)
	{
		double dWidth = Dom.GetTextWidth(m_strOpenType);
		double dHeight = Dom.GetTextHeight();
		Dom.TextOut(m_ptCen.x - dWidth/2, m_ptCen.y - dHeight/2, m_strOpenType);
	}

	*pDom << Dom;
}



void COpenBar::Serialize(CArchive& ar)
{
	long nFlag = 0;

	if(ar.IsStoring())  //Saving
	{
		ar << nFlag;
		ar << m_ptCen;			//개구부 중심위치 
		ar << m_nTypeOpen;		//개구부 타입(0 : 원형, 1:박스형)
		ar << m_dRadiusOpen;	//원형타입일때 개구부 반지름 
		ar << m_dWidthOpen;		//박스타입일때 개구부 가로길이
		ar << m_dLengthOpen;	//박스타입일때 개구부 세로길이 
		ar << m_strOpenType;	//개구부 종류 이름 
		ar << m_nCntRebar;		//한 방향의 철근 개수
		ar << m_dSpaceRebar;	//철근간격 
		ar << m_dDiaRebar;		//철근직경
		ar << m_dLengthRebar;	//철근전체 길이
		ar << m_dFy;			//철근항복강도 
	}
	else  //Open
	{
		ar >> nFlag;
		ar >> m_ptCen;			//개구부 중심위치 
		ar >> m_nTypeOpen;		//개구부 타입(0 : 원형, 1:박스형)
		ar >> m_dRadiusOpen;	//원형타입일때 개구부 반지름 
		ar >> m_dWidthOpen;		//박스타입일때 개구부 가로길이
		ar >> m_dLengthOpen;	//박스타입일때 개구부 세로길이 
		ar >> m_strOpenType;	//개구부 종류 이름 
		ar >> m_nCntRebar;		//한 방향의 철근 개수
		ar >> m_dSpaceRebar;	//철근간격 
		ar >> m_dDiaRebar;		//철근직경
		ar >> m_dLengthRebar;	//철근전체 길이
		ar >> m_dFy;		
	}
}

CString COpenBar::GetTextDia()
{
	CString strDia;
	
	strDia.Format("%c%0.0f", GetCharRebarMark(m_dFy), GetDiaRebar());
	
	return strDia;
}
