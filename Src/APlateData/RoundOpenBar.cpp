// RoundOpenBar.cpp: implementation of the CRoundOpenBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoundOpenBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define		COUNT_BARPOS		7		//�ö���� �� ��ġ ������ �� 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoundOpenBar::CRoundOpenBar()
{
	m_dRadius = 0;
	m_dAngle = 0;
	m_dAnchorageLength = 0;
}


CRoundOpenBar::~CRoundOpenBar()
{

}



void CRoundOpenBar::GetPosRebar(vector<CDPointArray>& vecUpper, vector<CDPointArray>& vecLower)
{
	vecUpper.clear();
	vecLower.clear();
	double dSpace = GetSpaceForCalc();

	/************************************************************************/
	/* ���� ó�� ö�� ��ġ�� ������ �´�.                                   */
	/************************************************************************/
	CDPointArray ptArrUpper, ptArrLower, ptArrTemp;
	GetPosFirstRebar(ptArrUpper, ptArrLower);
	vecUpper.push_back(ptArrUpper);
	vecLower.push_back(ptArrLower);

	/************************************************************************/
	/* ö���� ���� ��ŭ ��, �Ʒ� ö���� ���� ��Ų��.                        */
	/************************************************************************/
	for(int i = 0; i < m_nCntRebar- 1; i++)  
	{
		int j = 0;
		for(j = 0; j < ptArrUpper.GetSize(); j++)
		{
			CDPoint ptDot = ptArrUpper.GetAt(j);
			ptDot.y += dSpace;
			ptArrUpper.SetAt(j, ptDot);
		}

		for(j = 0; j < ptArrLower.GetSize(); j++)
		{
			CDPoint ptDot = ptArrLower.GetAt(j);
			ptDot.y += dSpace;
			ptArrLower.SetAt(j, ptDot);
		}

		vecUpper.push_back(ptArrUpper);
		vecLower.push_back(ptArrLower);
	}
}


//ȣ�� ���̸� �����Ѵ�.
double CRoundOpenBar::GetLengthArc()
{
	double dLenLine = GetLengthLine();
	double dLenTotal = GetLengthRebar();
	double dLen = dLenTotal - dLenLine*2;
	return dLen;
}


//�����ο��� ���� ����� �� ���� ö���� �����Ѵ�.
void CRoundOpenBar::GetPosFirstRebar(CDPointArray& ptArrUpper, CDPointArray& ptArrLower)
{
	ptArrUpper.RemoveAll();
	ptArrLower.RemoveAll();

	CDPoint ptCen(0,0);
	CDPoint ptDot[COUNT_BARPOS];
	CDPoint ptAngle[4];
	double dLine = GetLengthLine();  //�������̸� �����ϴ� ������ ���� ..
	double dAnchorageLen = GetLengthAnchorage();

	ptAngle[0] = ToDPointFrDegree(180 + m_dAngle/2);
	ptAngle[1] = ToDPointFrDegree(90 + m_dAngle/2);
	ptAngle[2] = ToDPointFrDegree(90 - m_dAngle/2);
	ptAngle[3] = ToDPointFrDegree(360 - m_dAngle/2);

	ptDot[2] = ptCen + (ptAngle[1]*m_dRadius);  //ó���� ptCen�� 0, 0 �̴�.
	ptDot[3] = ptCen + (ptAngle[2]*m_dRadius);
	ptDot[0] = ptDot[2] + (ptAngle[0]*dLine);
	ptDot[5] = ptDot[3] + (ptAngle[3]*dLine);
	ptDot[1] = ptDot[0] + ((ptDot[2] - ptDot[0]).Unit()*dAnchorageLen);
	ptDot[4] = ptDot[5] + ((ptDot[5] - ptDot[3]).Unit()*dAnchorageLen);
	ptDot[6] = ptCen;

	for(int i = 0; i < COUNT_BARPOS; i++)
		ptArrUpper.Add(ptDot[i]);

	ptArrLower = GetPosMirrorRebar(ptArrUpper);
}


double CRoundOpenBar::GetLengthLine()
{
	double dLenArc = RoundUp(2*ConstPi*m_dRadius*(m_dAngle/360), 0);
	double dLen = RoundUp((m_dLengthRebar - dLenArc)/2, 0);
	return dLen;
}

//ö�� ������ �밢���̱� ������ ���Ʒ��� ���� ö�ٰ����� �����;� �Ѵ�.
double CRoundOpenBar::GetSpaceForCalc()
{
	double dLen = m_dSpaceRebar / (cos( ToRadian(m_dAngle/2) ));
	return dLen;
}


void CRoundOpenBar::SetDataDefault(BOOL bRecommend)
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
		m_dLengthRebar = 2000;	
		m_dRadiusOpen = 100;

		m_dRadius = 200;		//��κ� ������ 
		m_dAngle = 50;			//��κ� ����
		m_dAnchorageLength = 300;		//�������� ����
	}
}



CDPointArray CRoundOpenBar::GetPosMirrorRebar(const CDPointArray &ptArr)
{
	CDPoint ptDot[COUNT_BARPOS], ptMirror[COUNT_BARPOS];
	CDPointArray ptArrMirror;
	double dSpace = 0;

	int nSize = ptArr.GetSize();
	int i = 0 ;
	for(i = 0; i < nSize; i++)
		ptDot[i] = ptArr.GetAt(i);

	double dHeightStd = ptDot[1].y;
	//0
	ptMirror[0].x = ptDot[0].x;
	dSpace = fabs(dHeightStd - ptDot[0].y);
	ptMirror[0].y = dHeightStd + dSpace;
	//1
	ptMirror[1] = ptDot[1];
	//2
	ptMirror[2].x = ptDot[2].x;
	dSpace = fabs(dHeightStd - ptDot[2].y);
	ptMirror[2].y = dHeightStd - dSpace;
	//3
	ptMirror[3].x = ptDot[3].x;
	dSpace = fabs(dHeightStd - ptDot[3].y);
	ptMirror[3].y = dHeightStd - dSpace;
	//4
	ptMirror[4] = ptDot[4];
	//5
	ptMirror[5].x = ptDot[5].x;
	dSpace = fabs(dHeightStd - ptDot[5].y);
	ptMirror[5].y = dHeightStd + dSpace;
	//6 Center
	ptMirror[6].x = ptDot[6].x;
	dSpace = dHeightStd - ptDot[6].y;
	ptMirror[6].y = dHeightStd + dSpace;

	for(i =0; i < nSize; i++)
		ptArrMirror.Add(ptMirror[i]);

	return ptArrMirror;

}



void CRoundOpenBar::Serialize(CArchive &ar)
{
	long nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_dRadius;			//��κ� ������ 
		ar << m_dAngle;				//��κ� ����
		ar << m_dAnchorageLength;	//�������� 
	}
	else
	{
		ar >> nFlag;
		ar >> m_dRadius;			//��κ� ������ 
		ar >> m_dAngle;				//��κ� ����
		ar >> m_dAnchorageLength;	//�������� 
	}

	COpenBar::Serialize(ar);
}
