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
	m_ptCen = obj.m_ptCen;		//������ �߽���ġ 
	m_nTypeOpen = obj.m_nTypeOpen;	//������ Ÿ��(0 : ����, 1:�ڽ���)
	m_dRadiusOpen = obj.m_dRadiusOpen;	//����Ÿ���϶� ������ ������ 
	m_dWidthOpen = obj.m_dWidthOpen;	//�ڽ�Ÿ���϶� ������ ���α���
	m_dLengthOpen = obj.m_dLengthOpen;	//�ڽ�Ÿ���϶� ������ ���α��� 
	m_strOpenType = m_strOpenType;	//������ ���� �̸� 

	m_nCntRebar = m_nCntRebar;	//�� ������ ö�� ����
	m_dSpaceRebar = m_dSpaceRebar;	//ö�ٰ��� 
	m_dDiaRebar = m_dDiaRebar;	//ö������
	m_dLengthRebar = m_dLengthRebar;	//ö����ü ����
	m_dFy = obj.m_dFy;
}


COpenBar::~COpenBar()
{

}



//�����θ� �׸���.
void COpenBar::DrawOpen(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	if(m_nTypeOpen == 0)  //����Ÿ�� 
	{
		Dom.Circle(m_ptCen, m_dRadiusOpen);		
	}
	else if(m_nTypeOpen == 1)  //�ڽ�Ÿ�� 
	{
		CDPoint ptLT(m_ptCen.x - m_dWidthOpen/2, m_ptCen.y + m_dLengthOpen/2);
		CDPoint ptRB(m_ptCen.x + m_dWidthOpen/2, m_ptCen.y - m_dLengthOpen/2);
		Dom.Rectangle(ptLT, ptRB);

		// X ǥ�� 
		Dom.LineTo(ptLT, ptRB);
		Dom.LineTo(ptLT.x + m_dWidthOpen, ptLT.y, ptRB.x - m_dWidthOpen, ptRB.y);
	}

	//���ڿ� ���
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
		ar << m_ptCen;			//������ �߽���ġ 
		ar << m_nTypeOpen;		//������ Ÿ��(0 : ����, 1:�ڽ���)
		ar << m_dRadiusOpen;	//����Ÿ���϶� ������ ������ 
		ar << m_dWidthOpen;		//�ڽ�Ÿ���϶� ������ ���α���
		ar << m_dLengthOpen;	//�ڽ�Ÿ���϶� ������ ���α��� 
		ar << m_strOpenType;	//������ ���� �̸� 
		ar << m_nCntRebar;		//�� ������ ö�� ����
		ar << m_dSpaceRebar;	//ö�ٰ��� 
		ar << m_dDiaRebar;		//ö������
		ar << m_dLengthRebar;	//ö����ü ����
		ar << m_dFy;			//ö���׺����� 
	}
	else  //Open
	{
		ar >> nFlag;
		ar >> m_ptCen;			//������ �߽���ġ 
		ar >> m_nTypeOpen;		//������ Ÿ��(0 : ����, 1:�ڽ���)
		ar >> m_dRadiusOpen;	//����Ÿ���϶� ������ ������ 
		ar >> m_dWidthOpen;		//�ڽ�Ÿ���϶� ������ ���α���
		ar >> m_dLengthOpen;	//�ڽ�Ÿ���϶� ������ ���α��� 
		ar >> m_strOpenType;	//������ ���� �̸� 
		ar >> m_nCntRebar;		//�� ������ ö�� ����
		ar >> m_dSpaceRebar;	//ö�ٰ��� 
		ar >> m_dDiaRebar;		//ö������
		ar >> m_dLengthRebar;	//ö����ü ����
		ar >> m_dFy;		
	}
}

CString COpenBar::GetTextDia()
{
	CString strDia;
	
	strDia.Format("%c%0.0f", GetCharRebarMark(m_dFy), GetDiaRebar());
	
	return strDia;
}
