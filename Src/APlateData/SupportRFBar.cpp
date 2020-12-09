// SupportRFBar.cpp: implementation of the CSupportRFBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SupportRFBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSupportRFBar::CSupportRFBar()
{
	m_dDia = 0;
	m_dLength = 0;
	m_dArcAngle = 0;
	m_dArcRadius = 0;
	m_ptArrStud.RemoveAll();
}


CSupportRFBar::CSupportRFBar(const CSupportRFBar& obj)
{
	m_dDia = obj.m_dDia;
	m_dLength = obj.m_dLength;
	m_dArcAngle = obj.m_dArcAngle;
	m_dArcRadius = obj.m_dArcRadius;
	m_ptCenter = obj.m_ptCenter;
	m_ptArrStud = obj.m_ptArrStud;
}


CSupportRFBar::~CSupportRFBar()
{

}


CSupportRFBar& CSupportRFBar::operator=(const CSupportRFBar& obj)
{
	m_dDia = obj.m_dDia;
	m_dLength = obj.m_dLength;
	m_dArcAngle = obj.m_dArcAngle;
	m_dArcRadius = obj.m_dArcRadius;
	m_ptCenter = obj.m_ptCenter;
	m_ptArrStud = obj.m_ptArrStud;

	return *this;
}


double CSupportRFBar::GetStudLength()
{
	double dLen = 0;

	int nSize = m_ptArrStud.GetSize();
	if(nSize > 2)
	{
		CDPoint ptStt = m_ptArrStud.GetAt(0);
		CDPoint ptEnd = m_ptArrStud.GetAt(nSize - 1);
		dLen = ~(ptStt - ptEnd);
	}

	return dLen;
}

double CSupportRFBar::GetArcLength()
{
	double dLenTotal = GetTotalLength();
	double dLenLine = GetLineLength();
	double dLen = dLenTotal - dLenLine*2;
	return dLen;
}

double CSupportRFBar::GetLineLength()
{
	double dLenArc = RoundUp(2*ConstPi*m_dArcRadius*(m_dArcAngle/360), 0);
	double dLenTotal = GetTotalLength();
	double dLen = RoundUp((dLenTotal - dLenArc)/2, 0);
	return dLen;
}


//원점을 기준으로 계산된 Stud와 철근의 교차점을 구한다.
void CSupportRFBar::GetXyMatchStudAndRebar(CDPoint &ptUpper, CDPoint &ptLower, BOOL bLeft)
{
	double dRest = 20; //스터드와 철근이 붙어 있으면 안되므로, 20정도의 여유를 더 준다.
	double dAngle = ToRadian(GetArcAngle()/2);
	double dAngle2 = ToRadian(90 - GetArcAngle()/2);
	double dStudLength = GetStudLength()/2 + dRest;
	double dX = 0, dY = 0;

	if(bLeft)
	{
		dX = -GetArcRadius()/cos(dAngle) + dStudLength/(tan(dAngle2));
		dY = dStudLength;
	}
	else
	{
		dX = GetArcRadius()/cos(dAngle) - dStudLength/(tan(dAngle2));
		dY = dStudLength;
	}

	ptUpper.x = dX;
	ptUpper.y = dY;
	ptLower.x = dX;
	ptLower.y = -dY;
}


//원점(0,0)을 기준으로한 철근 위치정보를 구한다.
//bLeft : TRUE->왼쪽철근, FALSE->오른쪽 철근 
CDPointArray CSupportRFBar::GetPosOrgRebar(BOOL bLeft)
{
	CDPoint ptCen(0,0);  //제일 처음은 (0,0)에서 시작한다.
	CDPointArray ptArr;
	CDPoint ptAngle[4];
	CDPoint ptDot[4];
	double dAngle = GetArcAngle()/2;

	if(bLeft == TRUE)
	{
		ptAngle[1] = ToDPointFrDegree(180 + dAngle);
		ptDot[1] = ptCen + ptAngle[1].Unit()*GetArcRadius();
		ptAngle[2] = ToDPointFrDegree(180 - dAngle);
		ptDot[2] = ptCen + ptAngle[2].Unit()*GetArcRadius();
		ptAngle[0] = ToDPointFrDegree(270 + dAngle);
		ptDot[0] = ptDot[1] + ptAngle[0].Unit()*GetLineLength();
		ptAngle[3] = ToDPointFrDegree(90 - dAngle);
		ptDot[3] = ptDot[2] + ptAngle[3].Unit()*GetLineLength();
		for(int i = 0; i < 4; i++)
			ptArr.Add(ptDot[i]);
	}
	else
	{
		ptAngle[1] = ToDPointFrDegree(-dAngle);
		ptDot[1] = ptCen + ptAngle[1].Unit()*GetArcRadius();
		ptAngle[2] = ToDPointFrDegree(dAngle);
		ptDot[2] = ptCen + ptAngle[2].Unit()*GetArcRadius();
		ptAngle[0] = ToDPointFrDegree(270 - dAngle);
		ptDot[0] = ptDot[1] + ptAngle[0].Unit()*GetLineLength();
		ptAngle[3] = ToDPointFrDegree(90 + dAngle);
		ptDot[3] = ptDot[2] + ptAngle[3].Unit()*GetLineLength();
		for(int i = 0; i < 4; i++)
			ptArr.Add(ptDot[i]);
	}

	return ptArr;
}


//원점으로 계산된 철근 위치를 실제 STUD 센터점으로 옮긴다.
void CSupportRFBar::MoveOrgBarToStudCen(CDPointArray &ptArr, BOOL bLeft)
{
	CDPointArray ptArrDot;
	CDPoint ptOrgStudCen, ptStudCen;
	ptOrgStudCen = GetOrgStudCenter(bLeft);  //좌, 우측 철근에 대한 원점기준 STUD의 CENTER위치를 구한다.
	ptStudCen = GetCalcStudCenter();  //실제 STUD의 CENTER위치를 구한다.
	
	double dRestX = ptStudCen.x - ptOrgStudCen.x;
	double dRestY = ptStudCen.y - ptOrgStudCen.y;

	int nSize = ptArr.GetSize();  //원점기준과 실제 STUD의 위치의 차이만큼 철근을 이동한다.
	for(int i = 0; i < nSize ; i++)
	{
		CDPoint ptDot = ptArr.GetAt(i);
		ptDot.x += dRestX;
		ptDot.y += dRestY;
		ptArrDot.Add(ptDot);
	}

	ptArr.RemoveAll();
	ptArr = ptArrDot;
}


void CSupportRFBar::DrawRebar(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	CDPointArray ptArrLeft = GetPosRebar(TRUE);
	CDPointArray ptArrRight = GetPosRebar(FALSE);
	CDPoint ptDot[4], ptCenArc;
	CRebarPlacing rb;
	double dAng = GetAngleStud();
	double dBulge = 0;
	double dRadius = GetArcRadius();
	int i = 0;
	/************************************************************************/
	/* 좌측                                                                 */
	/************************************************************************/
	for(i = 0; i < ptArrLeft.GetSize(); i++)
		ptDot[i] = ptArrLeft.GetAt(i);
	Dom.LineTo(ptDot[0], ptDot[1]);
	Dom.LineTo(ptDot[2], ptDot[3]);
	if(dAng != 0)
	{
		dBulge = GetArcBulge(ptDot[1], ptDot[2], dRadius);
		ptCenArc = GetCenterArc(ptDot[1], ptDot[2], dBulge);
	}
	else
		ptCenArc = GetArcCenter(ptArrLeft, TRUE);

	Dom.Arc(ptCenArc, dRadius, 180 - GetArcAngle()/2 + dAng, GetArcAngle());

	/************************************************************************/
	/* 우측                                                                 */
	/************************************************************************/
	for(i = 0; i < ptArrRight.GetSize(); i++)
		ptDot[i] = ptArrRight.GetAt(i);

	Dom.LineTo(ptDot[0], ptDot[1]);
	Dom.LineTo(ptDot[2], ptDot[3]);
	if(dAng != 0)
	{
		dBulge = GetArcBulge(ptDot[2], ptDot[1], dRadius);
		ptCenArc = GetCenterArc(ptDot[2], ptDot[1], dBulge);
	}
	else
		ptCenArc = GetArcCenter(ptArrRight, FALSE);

	Dom.Arc(ptCenArc, dRadius, 360 - GetArcAngle()/2 + dAng, GetArcAngle());

	*pDom << Dom;
}


CDPoint CSupportRFBar::GetArcCenter(CDPointArray ptArrPos, BOOL bLeft)
{
	CDPoint ptCen;

	if(ptArrPos.GetSize() > 1)
	{
		CDPoint ptDot = ptArrPos.GetAt(1);

		if(bLeft == TRUE)
		{
			CDPoint ptAngle = ToDPointFrDegree(GetArcAngle()/2);
			ptCen = ptDot + ptAngle.Unit()*GetArcRadius();		
		}
		else
		{
			CDPoint ptAngle = ToDPointFrDegree(180 - GetArcAngle()/2);
			ptCen = ptDot + ptAngle.Unit()*GetArcRadius();		
		}
	}

	return ptCen;
}


void CSupportRFBar::SetDataDefault(BOOL bRecommend)
{
	if(bRecommend)
	{
		m_dDia = 16;
		m_dLength = 2000;
		m_dArcAngle = 50;
		m_dArcRadius = 100;
	}
}


//스터드 위치로 부터 계산된 센터위치 
CDPoint CSupportRFBar::GetCalcStudCenter()
{
	int nSize = m_ptArrStud.GetSize();
	CDPoint ptCen;

	if(nSize > 0)
	{
		CDPoint ptStt = m_ptArrStud.GetAt(0);
		CDPoint ptEnd = m_ptArrStud.GetAt(nSize-1);
		ptCen = (ptStt + ptEnd)/2;
	}

	return ptCen;
}


void CSupportRFBar::DrawStud(CDomyun *pDom, double dRadius, double dDis)
{
	CDomyun Dom(pDom);

	for(int i = 0; i < m_ptArrStud.GetSize(); i++)
	{
		CDPoint ptCen = m_ptArrStud.GetAt(i);
		ptCen.x += dDis;
		Dom.Circle(ptCen, dRadius);
	}

	*pDom << Dom;
}


CDPointArray CSupportRFBar::GetPosRebar(BOOL bLeft)
{
	CDPointArray ptArrPos;
	ptArrPos = GetPosOrgRebar(bLeft);		//원점을 기준으로한 철근위치 정보 가져오기 
	MoveOrgBarToStudCen(ptArrPos, bLeft);	//철근정보를 Stud 중심으로 옮기기..
	RotateRebar(ptArrPos);
	return ptArrPos;
}


void CSupportRFBar::Serialize(CArchive &ar)
{
	int nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_dDia;
		ar << m_dLength;
		ar << m_dArcAngle;
		ar << m_dArcRadius;
		ar << m_ptCenter;
	}
	else
	{
		ar >> nFlag;
		ar >> m_dDia;
		ar >> m_dLength;
		ar >> m_dArcAngle;
		ar >> m_dArcRadius;
		ar >> m_ptCenter;
	}
	m_ptArrStud.Serialize(ar);
}

void CSupportRFBar::DrawDetail(CDomyun *pDom, double dDis)
{
	CDomyun Dom(pDom);

	CDPointArray ptArrLeft = GetPosDetailRebar(TRUE, dDis);
	CDPointArray ptArrRight = GetPosDetailRebar(FALSE, dDis);
	CDPoint ptDot[4];
	int i = 0;
	/************************************************************************/
	/* LEFT                                                                 */
	/************************************************************************/
	for(i = 0; i < ptArrLeft.GetSize(); i++)
		ptDot[i] = ptArrLeft.GetAt(i);

	Dom.LineTo(ptDot[0], ptDot[1]);
	Dom.LineTo(ptDot[2], ptDot[3]);
	Dom.Arc(GetArcCenter(ptArrLeft, TRUE), GetArcRadius(), 180 - GetArcAngle()/2, GetArcAngle());

	//OffSet
	ptDot[0].x -= GetDia();
	ptDot[1].x -= GetDia();
	ptDot[2].x -= GetDia();
	ptDot[3].x -= GetDia();

	Dom.LineTo(ptDot[0], ptDot[1]);
	Dom.LineTo(ptDot[2], ptDot[3]);
	ptArrLeft.SetAt(0, ptDot[0]);
	ptArrLeft.SetAt(1, ptDot[1]);
	ptArrLeft.SetAt(2, ptDot[2]);
	ptArrLeft.SetAt(3, ptDot[3]);
	Dom.Arc(GetArcCenter(ptArrLeft, TRUE), GetArcRadius(), 180 - GetArcAngle()/2, GetArcAngle());
	
	/************************************************************************/
	/* RIGHT                                                                */
	/************************************************************************/
	for(i = 0; i < ptArrRight.GetSize(); i++)
		ptDot[i] = ptArrRight.GetAt(i);

	Dom.LineTo(ptDot[0], ptDot[1]);
	Dom.LineTo(ptDot[2], ptDot[3]);
	Dom.Arc(GetArcCenter(ptArrRight, FALSE), GetArcRadius(), 360 - GetArcAngle()/2, GetArcAngle());

	//OffSet
	ptDot[0].x += GetDia();
	ptDot[1].x += GetDia();
	ptDot[2].x += GetDia();
	ptDot[3].x += GetDia();

	Dom.LineTo(ptDot[0], ptDot[1]);
	Dom.LineTo(ptDot[2], ptDot[3]);
	ptArrRight.SetAt(0, ptDot[0]);
	ptArrRight.SetAt(1, ptDot[1]);
	ptArrRight.SetAt(2, ptDot[2]);
	ptArrRight.SetAt(3, ptDot[3]);
	Dom.Arc(GetArcCenter(ptArrRight, FALSE), GetArcRadius(), 360 - GetArcAngle()/2, GetArcAngle());

	*pDom << Dom;
}


void CSupportRFBar::DrawDetailCircle(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	//1. 상세 원을 그린다. 
	CDPoint ptCen = GetCalcStudCenter();
	double dRadius = GetDetailRadius();
	Dom.Circle(ptCen, dRadius);

	*pDom << Dom;
}


double CSupportRFBar::GetDetailRadius()
{
	double dRadius = 0;
	CDPointArray ptArrStud = GetPosStud();
	int nCntStud = ptArrStud.GetSize();
	if(nCntStud <= 0) return dRadius;

	CDPoint ptStt = ptArrStud.GetAt(0);
	CDPoint ptEnd = ptArrStud.GetAt(nCntStud - 1);
	dRadius = ~(ptStt - ptEnd);
	dRadius += dRadius*0.2;

	return dRadius;
}


CDPointArray CSupportRFBar::GetPosDetailRebar(BOOL bLeft, double dDis)
{
	CDPoint ptCen = GetCalcStudCenter();
	double dRadius = GetDetailRadius();

	CDPointArray ptArrDot;
	CDPoint ptUpper, ptLower;
	CDPoint ptDot[4];
	ptArrDot = GetPosRebar(bLeft);
	ptDot[0] = ptArrDot[0];
	ptDot[1] = ptArrDot[1];
	ptDot[2] = ptArrDot[2];
	ptDot[3] = ptArrDot[3];

	GetXyMatchSegAndLine(ptDot[0], ptDot[1], CDPoint(0, ptCen.y - dRadius), CDPoint(1, 0), ptLower);
	GetXyMatchSegAndLine(ptDot[2], ptDot[3], CDPoint(0, ptCen.y + dRadius), CDPoint(1, 0), ptUpper);

	ptArrDot.SetAt(0, ptLower);
	ptArrDot.SetAt(3, ptUpper);

	for(int i = 0; i < ptArrDot.GetSize(); i++)
	{
		ptArrDot.GetAt(i).x += dDis;
	}

	return ptArrDot;
}


void CSupportRFBar::MarkStud(CDomyun *pDom, double dDis)
{
	CDomyun Dom(pDom);

	CString strStud = "STUD";
	CDPointArray ptArrStud = GetPosStud(dDis);
	double dRadius = GetDetailRadius();
	CDPoint ptDot = ptArrStud.GetAt(0);
	Dom.GiSiArrowExt(ptDot.x, ptDot.y, dRadius, Dom.GetTextWidth(strStud)*2, 0, strStud);

	*pDom << Dom;
}


void CSupportRFBar::DrawDetailCircleText(CDomyun *pDom, CString strText)
{
	CDomyun Dom(pDom);

	Dom.SetDirection("BOTTOM");

	CDPoint ptCen = GetCalcStudCenter();
	double dRadius = GetDetailRadius();
	CDPoint ptDot;
	ptDot.x = ptCen.x + dRadius;
	ptDot.y = ptCen.y;
	Dom.GiSiArrowExt(ptDot.x, ptDot.y, dRadius*2, Dom.GetTextWidth(strText)*2, 1, strText);

	*pDom << Dom;
}


CDPointArray CSupportRFBar::GetPosStud(double dDis)
{
	if(dDis == 0)
		return m_ptArrStud;
	else
	{
		CDPointArray ptArrStud;

		for(int i = 0; i < m_ptArrStud.GetSize(); i++)
		{
			CDPoint ptStud = m_ptArrStud.GetAt(i);
			ptStud.x += dDis;
			ptArrStud.Add(ptStud);
		}

		return ptArrStud;
	}
}


//원점을 기준으로한 Stud의 센터점의 위치를 구한다.
CDPoint CSupportRFBar::GetOrgStudCenter(BOOL bLeft)
{
	CDPoint ptOrgCen;
	double dRest = 20; //스터드와 철근이 붙어 있으면 안되므로, 20정도의 여유를 더 준다.
	double dAngle = ToRadian(GetArcAngle()/2);
	double dAngle2 = ToRadian(90 - GetArcAngle()/2);
	double dStudLength = GetStudLength()/2 + dRest;

	if(bLeft)
		ptOrgCen.x = -GetArcRadius()/cos(dAngle) + dStudLength/(tan(dAngle2));
	else
		ptOrgCen.x = GetArcRadius()/cos(dAngle) - dStudLength/(tan(dAngle2));

	ptOrgCen.y = 0;

	return ptOrgCen;
}


//STUD의 위치에 각이 있다면, 그 각만큼 회전시킨다.
//이때 회전의 중심점은 STUD의 CENTER이다.
void CSupportRFBar::RotateRebar(CDPointArray &ptArr)
{
	double dAng = GetAngleStud();
	if(dAng == 0)
		return;

	CRebarPlacing rb;
	CDPoint ptCen = GetCalcStudCenter();

	for(int i = 0; i < ptArr.GetSize(); i++)
	{
		CDPoint ptDot = ptArr.GetAt(i);
		rb.RotateXy(ptDot, ptCen, dAng);
		ptArr.SetAt(i, ptDot);
	}
}


double CSupportRFBar::GetAngleStud()
{
	double dAng = 0;
	CDPointArray ptArrStud = GetPosStud();
	int nSize = ptArrStud.GetSize();
	if(nSize <= 0)	
		return dAng;

	CDPoint ptStt = ptArrStud.GetAt(0);
	CDPoint ptEnd = ptArrStud.GetAt(nSize - 1);
	if(ptStt.x == ptEnd.x)
		return dAng;

	dAng = (ptEnd - ptStt).Unit().GetAngleDegree();
	if(dAng < 90)
		dAng = -(90-dAng);
	else
		dAng = dAng - 90;

	return dAng;
}

void CSupportRFBar::SetPosStud(CDPointArray ptArrStud)
{
	int nSize = ptArrStud.GetSize();
	if(nSize <= 0)
		return;

	m_ptArrStud = ptArrStud;

//	CDPoint ptStt = ptArrStud.GetAt(0);
//	CDPoint ptEnd = ptArrStud.GetAt(nSize - 1);
//	if(ptStt.y > ptEnd.y)  //위에서 아래로 내려오는 형상이면 
//	{
//		vector<CDPoint> vecStud;
//
//		for(int i = nSize - 1; i > 0; i--)
//		{
//			CDPoint ptDot = ptArrStud.GetAt(i);
//			m_ptArrStud.Add(ptDot);
//		}
//	}
//	else
//		m_ptArrStud = ptArrStud;
}
