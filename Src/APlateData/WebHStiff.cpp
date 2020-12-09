// WebHStiff.cpp: implementation of the CWebHStiff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "WebHStiff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebHStiff::CWebHStiff()
{
	Clear();
}
void CWebHStiff::Clear()
{	
	m_nDanSu[POS_UP][WEB_LEFT] = m_nDanSu[POS_DN][WEB_LEFT] = 
	m_nDanSu[POS_UP][WEB_RIGHT] = m_nDanSu[POS_DN][WEB_RIGHT] = 0;	

	m_dThickHStiff[WEB_LEFT] = m_dThickHStiff[WEB_RIGHT] = 0;		//수평보강재 두께
	m_dWidthHStiff[WEB_LEFT] = m_dWidthHStiff[WEB_RIGHT] = 0;		//수평보강재 너비

	for(long i = 0; i < WEBCOUNT; i++)
		for(long n = 0; n < WEBHSTIFFSUMAX; n++)
		{
			m_dStationStt[i][n] = 0;		//좌측(시작) 스테이션
			m_dStationEnd[i][n] = 0;		//우측(종점) 스테이션
			m_dHeightFactor[i][n] = 0;	//높이 비율(상판 하단 기준 - 헌치부 포함 => +200)
			m_dLengthHStiff[i][n] = 0;	//실제 길이
			m_dDistStt[i][n] = 0;			//좌측 꺽임 길이(잭업보강재 사이에 있을 경우 등)
			m_dDistEnd[i][n] = 0;			//우측 꺽임 길이(잭업보강재 사이에 있을 경우 등)
		}
	m_nPos = WEB_SIDE_BOTH;
}

CWebHStiff::~CWebHStiff()
{

}

DWORD CWebHStiff::GetLinkType()
{	
	return 	 LK_WEBHSTIFF;	// default
}

void CWebHStiff::SetTotalDansu(BOOL bUpper, BOOL bLeft, long nDan)
{
	m_nDanSu[bUpper ? POS_UP : POS_DN][bLeft ? WEB_LEFT : WEB_RIGHT] = nDan;	
}

long CWebHStiff::GetTotalDansu(BOOL bUpper, BOOL bLeft) const
{
	return m_nDanSu[bUpper ? POS_UP : POS_DN][bLeft ? WEB_LEFT : WEB_RIGHT];
}

BOOL CWebHStiff::IsEmpty(long nDan, BOOL bLeft) const
{//수평보강재 두께 && 수평보강재 너비

	//m_nPos  0:양측 1:좌측 2:우측
	if(bLeft)
	{
		if(m_nPos == WEB_SIDE_RIGHT) return TRUE;
	}
	else
	{
		if(m_nPos == WEB_SIDE_LEFT) return TRUE;
	}
	

	return m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] == 0.0;
}

long CWebHStiff::GetUpperDan(BOOL bLeft) const
{
	long nRetDan = 0;
	for(long nDan=0; nDan<3; nDan++)
		if(m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] != 0) nRetDan++;

	return nRetDan;
}

long CWebHStiff::GetLowerDan(BOOL bLeft) const
{
	long nRetDan = 0;
	for(long nDan=3; nDan<6; nDan++)
		if(m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] != 0) nRetDan++;

	return nRetDan;
}

double CWebHStiff::GetLength(long nDan, BOOL bLeft, DIMTYPE nDimType) const
{
	double dWidth = 0;
	double dPrevSta = m_dStationStt[bLeft ? WEB_LEFT : WEB_RIGHT][nDan];
	double dNextSta = m_dStationEnd[bLeft ? WEB_LEFT : WEB_RIGHT][nDan];
	double dLength = m_pGir->GetLengthDimType(dPrevSta, dNextSta, dWidth);
	if(nDimType==DIM_BASIC)
		return dLength;
//	double dLength = m_pGir->GetLengthDimType(dPrevSta, dNextSta, dWidth,TRUE,nDimType);
	double dPrevEle = m_pGir->GetElevationFlangeCamber(dPrevSta, dWidth) - GetHeightHStiffOnStt(nDan, bLeft, TRUE);
	double dNextEle = m_pGir->GetElevationFlangeCamber(dNextSta, dWidth) - GetHeightHStiffOnStt(nDan, bLeft, FALSE);
	return Round(hypot(dLength, dNextEle - dPrevEle), 1);
//	if (nDimType > 0)	//hgh 2001.11.03
//	{
//		double dPrevEle = m_pGir->GetElevationFlangeCamber(dPrevSta) - GetHeightHStiffOnStt(nDan, TRUE);
//		double dNextEle = m_pGir->GetElevationFlangeCamber(dNextSta) - GetHeightHStiffOnStt(nDan, FALSE);
//		dLength = hypot(dLength, dNextEle - dPrevEle);
//	}
//	return dLength;
}

//<summary>
// 실제 상판 하면에서 부터 수평보강재 두께 중앙까지의 거리를 구함.
// 실제 엘리베이션 개념이 아님. 테이퍼 구간에서 간격은 같지만 실제 위치는 다름.
//</summary>
//<param name="nDan"> 수평보강재 단수 1~6단</param>
//<param name="bStt"> 현재 수평보강재의 시작, 종점을 선택</param>
//<param name="sta/*=-1*/"></param>
//<return></return>
double CWebHStiff::GetHeightHStiffBySta(long nDan, BOOL bLeft, double sta) const
{
	if (m_dHeightFactor[nDan] == 0)
		return 0;
	CPlateBridgeApp *pDB = (CPlateBridgeApp *)m_pGir->GetBridgeApp();
//	if (sta < 0)
//		sta = bStt ? m_dStationStt[nDan] : m_dStationEnd[nDan];
	double height;
	if (pDB->m_cHStiffHeightOffset && nDan < WEBHSTIFFSUMAX / 2)   {
		height = m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan];
		double dUpperH = m_pGir->GetHeightFlangeUpperByTaper(sta, 0);
		if (dUpperH > 0)   {
			if (pDB->m_nHStiffDefault)   {
				double dUpperL = m_pGir->GetHeightFlangeLower(sta,0,FALSE);
				height *= (dUpperL - dUpperH) / dUpperL;
			}
			else
				height -= dUpperH;
		}
	}
	else   {
		height = m_pGir->GetHeightGirderByStaAng(sta, 0, CDPoint(0,1),FALSE) * m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan];
		if (!pDB->m_nHStiffDefault)   {
			double dUpperH = m_pGir->GetHeightFlangeUpperByTaper(sta, 0);
			if (dUpperH > 0)
				height = m_pGir->GetHeightFlangeLower(sta) * m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] - dUpperH;
		}
	}

	return height;
}

double CWebHStiff::GetHeightHStiffOnStt(long nDan, BOOL bLeft, BOOL bStt) const
{
	double sta = bStt ? m_dStationStt[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] : m_dStationEnd[bLeft ? WEB_LEFT : WEB_RIGHT][nDan];
	return GetHeightHStiffBySta(nDan, bLeft, sta);
}
void CWebHStiff::Serialize(CArchive& ar)
{
	long nFlag = 1;
	
	CPlateBxObject::Serialize(ar);
	long i = 0;
	long n = 0;
	if( ar.IsStoring() )
	{
		ar << nFlag;
		for(i = 0; i < 2; i++)
			for(n=0; n < WEBCOUNT; n++)
				ar << m_nDanSu[i][n];

		for(i = 0; i < WEBCOUNT; i++)	
		{
			ar << m_dThickHStiff[i];
			ar << m_dWidthHStiff[i];
			ar << WEBHSTIFFSUMAX;
			for(n = 0; n < WEBHSTIFFSUMAX; n++)
			{
				ar << m_dStationStt[i][n];
				ar << m_dStationEnd[i][n];
				ar << m_dHeightFactor[i][n];
				ar << m_dLengthHStiff[i][n];
				ar << m_dDistStt[i][n];
				ar << m_dDistEnd[i][n];
			}
		}
		ar << m_nPos;
	}
	else 
	{
		ar >> nFlag;	
		for(i = 0; i < 2; i++)
			for(n=0; n < WEBCOUNT; n++)
				ar >> m_nDanSu[i][n];

		for(i = 0; i < WEBCOUNT; i++)	
		{
			ar >> m_dThickHStiff[i];
			ar >> m_dWidthHStiff[i];
			long nWEBHSTIFFSUMAX;
			ar >> nWEBHSTIFFSUMAX;
			for(n = 0; n < nWEBHSTIFFSUMAX; n++)
			{
				ar >> m_dStationStt[i][n];
				ar >> m_dStationEnd[i][n];
				ar >> m_dHeightFactor[i][n];
				ar >> m_dLengthHStiff[i][n];			
				ar >> m_dDistStt[i][n];
				ar >> m_dDistEnd[i][n];						
			}
		}
		if(nFlag>0) ar >> m_nPos;
	}
}
CWebHStiff& CWebHStiff::operator=(const CWebHStiff& obj)
{
	if( this == &obj) return *this;

	CPlateBxObject::operator =(obj);
	long i =0;
	for(i = 0; i < 2; i++)
		for(long n=0; n < WEBCOUNT; n++)
			m_nDanSu[i][n] = obj.m_nDanSu[i][n];

	for(i = 0; i < WEBCOUNT; i++)
	{
		for(long n = 0; n < WEBHSTIFFSUMAX; n++)
		{
			m_dStationStt[i][n] = obj.m_dStationStt[i][n];
			m_dStationEnd[i][n] = obj.m_dStationEnd[i][n];
			m_dHeightFactor[i][n] = obj.m_dHeightFactor[i][n];
			m_dLengthHStiff[i][n] = obj.m_dLengthHStiff[i][n];
			m_dDistStt[i][n] = obj.m_dDistStt[i][n];
			m_dDistEnd[i][n] = obj.m_dDistEnd[i][n];
		}
		m_dThickHStiff[i] = obj.m_dThickHStiff[i];
		m_dWidthHStiff[i] = obj.m_dWidthHStiff[i];
	}
	m_nPos = obj.m_nPos;
	
	return *this;
}
//입력 : nDansu		0 = 상부에서 첫번째 단, 1=두번째 단, 3=세번째 단, 4 = 네번째 단
//기능 : 수평보강재의 각도(radian)를 리턴
double CWebHStiff::GetAngleRadianHStiff(long nRow, BOOL bLeft) const
{
	if(IsEmpty(nRow, bLeft))	return 0;

	CPlateGirderApp *pGir = GetBx()->GetGirder();
	double dWidth = 0;
	double staStt = m_dStationStt[bLeft ? WEB_LEFT : WEB_RIGHT][nRow];
	double staEnd = m_dStationEnd[bLeft ? WEB_LEFT : WEB_RIGHT][nRow];
	
	double dEleStt = pGir->GetElevationFlangeCamber(staStt,dWidth) - GetHeightHStiffOnStt(nRow, bLeft, TRUE);
	double dEleEnd = pGir->GetElevationFlangeCamber(staEnd,dWidth) - GetHeightHStiffOnStt(nRow, bLeft, FALSE);
	double dDis = pGir->GetLengthDimType(staStt, staEnd, dWidth);
	CDPoint angRad(dDis, dEleEnd - dEleStt);
	return angRad.Unit().GetAngleRadian();
}

void CWebHStiff::SetFillAssmVariable(CDoubleArray& dArr, BOOL bLeft)
{
	dArr.Add(m_dThickHStiff[bLeft ? WEB_LEFT : WEB_RIGHT]);
	dArr.Add(m_dWidthHStiff[bLeft ? WEB_LEFT : WEB_RIGHT]);

	for(long i=0;i<WEBHSTIFFSUMAX;i++)
	{
		dArr.Add(m_dLengthHStiff[bLeft ? WEB_LEFT : WEB_RIGHT][i]);	//실제 길이
		dArr.Add(m_dDistStt[bLeft ? WEB_LEFT : WEB_RIGHT][i]+m_dDistEnd[bLeft ? WEB_LEFT : WEB_RIGHT][i]);	//좌측 꺽임 길이(잭업보강재 사이에 있을 경우 등)
		//dArr.Add(m_dDistEnd[i]);
	}
}