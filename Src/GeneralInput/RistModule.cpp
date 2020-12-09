// RistModule.cpp: implementation of the CRistModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "generalinput.h"
#include "RistModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CRistModule::m_gFlangeWidthTitle[2][5] = {
	{"30m+40m+30m","40m+50m+40m","50m+60m+50m","55m+70m+55m","65m+80m+65m"},
	{"30m+40m+30m","40m+50m+40m","50m+60m+50m","55m+70m+55m","65m+80m+65m"}	
};

double CRistModule::m_dCalcJigan[5] = { 40,50,60,70,80 };

double CRistModule::m_gFlangeWidthValue[2][2][5] = { 
	{{33.3,33.3,45.7,53.3,76.7}, {100,100,262,367,750}},
	{{40, 50, 54.3, 66.7, 90}, {140,310,242,400,730}}
};

double CRistModule::m_gFlangeLengthValue[2][5][12] = {
	{{0.150, 0.134, 0.134, 0.313, 0.134, 0.134, 0.150, 0.228, 0.312, 0.185, 0.173, 0.200 },	
	{ 0.150, 0.150, 0.150, 0.250, 0.150, 0.150, 0.150, 0.218, 0.219, 0.219, 0.250, 0.188 },		
	{ 0.125, 0.167, 0.167, 0.208, 0.167, 0.167, 0.125, 0.174, 0.300, 0.250, 0.200, 0.150 },	
	{ 0.107, 0.179, 0.179, 0.179, 0.179, 0.179, 0.107, 0.249, 0.227, 0.227, 0.227, 0.136 },
	{ 0.094, 0.188, 0.188, 0.156, 0.188, 0.188, 0.094, 0.365, 0.192, 0.192, 0.192, 0.115 }},
	{{0.150, 0.134, 0.134, 0.313, 0.134, 0.134, 0.150, 0.228, 0.312, 0.185, 0.173, 0.200 },
	{ 0.150, 0.150, 0.150, 0.250, 0.150, 0.150, 0.150, 0.218, 0.219, 0.219, 0.250, 0.188 },		
	{ 0.125, 0.167, 0.167, 0.208, 0.167, 0.167, 0.125, 0.174, 0.300, 0.250, 0.200, 0.150 },	
	{ 0.107, 0.179, 0.179, 0.179, 0.179, 0.179, 0.107, 0.249, 0.227, 0.227, 0.227, 0.136 },	
	{ 0.094, 0.188, 0.188, 0.156, 0.188, 0.188, 0.094, 0.365, 0.192, 0.192, 0.192, 0.115 }}
};

double CRistModule::m_gFlangeUpperThickValue[2][5][12] = {
	{{50, 34, 34, 20, 34, 34, 50, 20, 20, 34, 34, 50 },	
	{ 58, 40, 22, 22, 22, 40, 58, 22, 22, 22, 40, 58 },		
	{ 66, 52, 38, 28, 38, 52, 66, 26, 28, 38, 52, 66 },	
	{ 58, 40, 32, 30, 32, 40, 58, 30, 32, 32, 40, 58 },
	{ 58, 40, 36, 34, 36, 40, 58, 34, 34, 36, 40, 58 }},
	{{48, 32, 20, 18, 20, 32, 48, 18, 18, 20, 32, 48 },
	{ 60, 42, 24, 22, 24, 42, 60, 20, 24, 24, 42, 60 },		
	{ 60, 46, 32, 28, 32, 46, 60, 28, 30, 32, 46, 60 },	
	{ 56, 38, 30, 30, 30, 38, 56, 28, 30, 30, 38, 56 },	
	{ 56, 38, 36, 34, 36, 38, 56, 30, 30, 36, 38, 56 }}
};

double CRistModule::m_gFlangeLowerThickValue[2][5][12] = {
	{{60, 42, 34, 22, 34, 42, 60, 22, 22, 34, 42, 60 },	
	{ 72, 54, 36, 22, 36, 54, 72, 22, 36, 36, 54, 72 },		
	{ 68, 54, 40, 30, 40, 54, 68, 28, 28, 40, 54, 68 },	
	{ 66, 48, 36, 34, 36, 48, 66, 34, 36, 36, 48, 66 },
	{ 68, 50, 38, 36, 38, 50, 68, 36, 36, 38, 50, 68 }},
	{{50, 34, 20, 20, 20, 34, 50, 20, 20, 20, 34, 50 },
	{ 70, 52, 34, 22, 34, 52, 70, 22, 34, 34, 52, 70 },		
	{ 66, 52, 38, 30, 38, 52, 66, 30, 30, 38, 52, 66 },	
	{ 66, 48, 30, 34, 30, 48, 66, 30, 30, 30, 48, 66 },	
	{ 64, 46, 36, 36, 36, 46, 64, 34, 34, 36, 46, 64 }}
};

double CRistModule::m_gWebThickValue[2][5][12] = {
	{{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },	
	{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },		
	{ 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 },	
	{ 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 },
	{ 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 }},
	{{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
	{ 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22 },		
	{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },	
	{ 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 },	
	{ 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 }}
};

CRistModule::CRistModule()
{

}

CRistModule::~CRistModule()
{

}

double CRistModule::GetLengthMaxJigan(long nQtyJigan, double *dLengthArray)
{
	double dMaxJigan = 0;
	for(long n=0; n<nQtyJigan; n++)
	{
		dMaxJigan = max(dMaxJigan, Round(dLengthArray[n],-3));
	}
	return dMaxJigan;
}

double CRistModule::GetHeightGirder(long nQtyJigan, double *dLengthArray, BOOL bSM520)
{
	double dMaxJigan = GetLengthMaxJigan(nQtyJigan, dLengthArray);
	return Round((bSM520 ? 0.04 : 0.035) * dMaxJigan + 700,-2);	
}

long CRistModule::GetDividCount(double dMaxJiGan, long nQtyJigan, BOOL bSM520)
{
	if(nQtyJigan==1) return 5;
	else if(nQtyJigan==2) return 11;
	else return 10 + (nQtyJigan-2) * 5;
}

long CRistModule::GetIndexNearByMaxJigan(double dMaxJiGan)
{
	long nIdx=0;
	for(long n=0; n<5; n++)
	{
		if(m_dCalcJigan[n] > toM(dMaxJiGan)) 
		{
			nIdx = n;
			break;
		}
	}
	return nIdx;
}

long CRistModule::GetRealPos(long nQtyJigan, long nCurJigan, long nPos)
{
	long nRealPos = nPos;
	if(nCurJigan==0)		        nRealPos = 7 + nPos;	
	else if(nCurJigan==nQtyJigan-1) nRealPos = 11 - nPos;	

	return nRealPos;
}

///< x1: ���۵�������ġ
///< y1: ���۵����Ͱ�
///< x2: ����������ġ
///< y2: �������Ͱ�
///< x : ������ ��ġ��
double CRistModule::Bogan(double x1, double x2, double y1, double y2, double x)
{
	double dResult = 0;

	if(fabs(x1-x2) < 0.001)
		return (y1 + y2) / 2;
	if(fabs(x2-x) < 0.001)
		return y2;

	dResult = y1 + (x - x1) * (y2 - y1) / (x2 - x1);
	return dResult;
}


double CRistModule::GetRatio(double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520)
{
	long nIdx = GetIndexNearByMaxJigan(dMaxJiGan);

	double dValueMin, dValueMax;
	double dJiganMin, dJiganMax;

	long nRealPos = GetRealPos(nQtyJigan, nCurJigan, nPos);
	
	if(nIdx==0 || nIdx==4)	
	{
		dValueMin = dValueMax = m_gFlangeLengthValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		dJiganMin = dJiganMax = m_dCalcJigan[nIdx];
	}
	else
	{
		dValueMin = m_gFlangeLengthValue[(bSM520 ? 0 : 1)][nIdx-1][nRealPos];
		dValueMax = m_gFlangeLengthValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		dJiganMin = m_dCalcJigan[nIdx-1];
		dJiganMax = m_dCalcJigan[nIdx];
	}	
	double dValue = Bogan(dJiganMin,dJiganMax,dValueMin,dValueMax,toM(dMaxJiGan));	
	return ((nCurJigan==0 && nPos==4) || 
		    (nCurJigan==nQtyJigan-1 && nPos==0) || 
			(nCurJigan>0 && nCurJigan<nQtyJigan-1 && (nPos==0 || nPos==6))) ? dValue/2 : dValue;	
}

double CRistModule::GetWebThick(double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520)
{
	long nIdx = GetIndexNearByMaxJigan(dMaxJiGan);

	double dValueMin, dValueMax;
	double dJiganMin, dJiganMax;

	long nRealPos = GetRealPos(nQtyJigan, nCurJigan, nPos);
	
	if(nIdx==0 || nIdx==4)	
	{
		dValueMin = dValueMax = m_gWebThickValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		dJiganMin = dJiganMax = m_dCalcJigan[nIdx];
	}
	else
	{
		dValueMin = m_gWebThickValue[(bSM520 ? 0 : 1)][nIdx-1][nRealPos];
		dValueMax = m_gWebThickValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		dJiganMin = m_dCalcJigan[nIdx-1];
		dJiganMax = m_dCalcJigan[nIdx];
	}

	//�β��� ¦���� ���;� �Ѵ�...
	//������� 20.6 �� ������ 20���� �ϰ� 21.6�� ������ 21�� �ϰ� +1 22�� ����...
	long dResult = long(Bogan(dJiganMin,dJiganMax,dValueMin,dValueMax,toM(dMaxJiGan)));
	
	if((dResult)%2==1) dResult += 1;

	return double(dResult);	
}


double CRistModule::GetFlangeThick(double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520, BOOL bUpper)
{
	long nIdx = GetIndexNearByMaxJigan(dMaxJiGan);

	double dValueMin, dValueMax;
	double dJiganMin, dJiganMax;

	long nRealPos = GetRealPos(nQtyJigan, nCurJigan, nPos);
	
	if(nIdx==0 || nIdx==4)	
	{
		if(bUpper)	dValueMin = dValueMax = m_gFlangeUpperThickValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		else		dValueMin = dValueMax = m_gFlangeLowerThickValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		dJiganMin = dJiganMax = m_dCalcJigan[nIdx];
	}
	else
	{
		if(bUpper)	
		{		
			dValueMin = m_gFlangeUpperThickValue[(bSM520 ? 0 : 1)][nIdx-1][nRealPos];
			dValueMax = m_gFlangeUpperThickValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		}
		else
		{
			dValueMin = m_gFlangeLowerThickValue[(bSM520 ? 0 : 1)][nIdx-1][nRealPos];
			dValueMax = m_gFlangeLowerThickValue[(bSM520 ? 0 : 1)][nIdx][nRealPos];
		}
		dJiganMin = m_dCalcJigan[nIdx-1];
		dJiganMax = m_dCalcJigan[nIdx];
	}

	//�β��� ¦���� ���;� �Ѵ�...
	//������� 20.6 �� ������ 20���� �ϰ� 21.6�� ������ 21�� �ϰ� +1 22�� ����...
	long dResult = long(Bogan(dJiganMin,dJiganMax,dValueMin,dValueMax,toM(dMaxJiGan)));
	
	if((dResult)%2==1) dResult += 1;

	return double(dResult);
}

double CRistModule::GetFlangeWidth(double dGirderH, double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520, BOOL bUpper)
{
	long nIdx = GetIndexNearByMaxJigan(dMaxJiGan);
	
	double dValueMin, dValueMax;
	double dJiganMin, dJiganMax;
	long nFirst = (bSM520 ? 1 : 0);//������ �ٲ�� ����.. ���� ..070125..KB

	if(nIdx==0 || nIdx==4)	
	{
		dValueMin = dValueMax = m_gFlangeWidthValue[nFirst][0][nIdx] * dGirderH/dMaxJiGan + m_gFlangeWidthValue[nFirst][1][nIdx];
		dJiganMin = dJiganMax = m_dCalcJigan[nIdx];
	}
	else
	{
		dValueMin = m_gFlangeWidthValue[nFirst][0][nIdx-1] * dMaxJiGan/dGirderH - m_gFlangeWidthValue[nFirst][1][nIdx-1];
		dValueMax = m_gFlangeWidthValue[nFirst][0][nIdx] * dMaxJiGan/dGirderH - m_gFlangeWidthValue[nFirst][1][nIdx]; 
		dJiganMin = m_dCalcJigan[nIdx-1];
		dJiganMax = m_dCalcJigan[nIdx];
	}	
	double dValue = Bogan(dJiganMin,dJiganMax,dValueMin,dValueMax,toM(dMaxJiGan));
	dValue = bUpper ? dValue : dValue*1.1;//������ ��� 10%����..
	double dResult = bUpper ? RoundDown(dValue,-2) : RoundDown(dValue,-2);
	double dGap = fabs(dValue - dResult);

	//�÷������� 500, 550, 600, 650...ó�� 50���������ϱ� ���ؼ�... 061228...KB
	//if (dGap < 25)
	if (dGap >= 25 && dGap < 75)
		dResult = dResult +50;
	else if(dGap >= 75)
		dResult = dResult +100;

	return dResult;
}

void CRistModule::DrawRistGraph(CDomyun *pDom, double dCurJigan, double dCurH, BOOL bSM520)
{
	CDomyun Dom(pDom);
	double dXScale=20;
	
	Dom.SetLineColor(7);		
	Dom.LineTo(1*dXScale,30,5*dXScale,30);	// X��
	Dom.LineTo(1*dXScale,30,1*dXScale,90);	// Y��

	Dom.SetTextColor(3);
	Dom.SetTextHeight(3);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.TextOut(3*dXScale,100,"������ �����");
	Dom.SetTextHeight(2);
	if(bSM520)	Dom.TextOut(3*dXScale,96,"(0.04 x a + 0.7)");
	else		Dom.TextOut(3*dXScale,96,"(0.035 x a + 0.7)");

	Dom.SetTextColor(6);	
	// ����
	Dom.SetTextHeight(2);	
	Dom.TextOut(-10+1*dXScale,55,"����");
	Dom.TextOut(3*dXScale,22,"����");

	Dom.SetTextHeight(1.5);
	// X�� ����
	long n = 0;
	for(n=1; n<6; n++)
	{
		Dom.LineTo(n*dXScale,30,n*dXScale,29);
		Dom.TextOut(n*dXScale,27,atofRound((long)n,0));
	}

	// Y�� ����
	for(n=0; n<7; n++)
	{
		Dom.LineTo(-1+1*dXScale,30+n*10,1*dXScale,30+n*10);
		Dom.TextOut(-3+1*dXScale,30+n*10,atofRound((long)(30+n*10),0));
	}

	// ���� �׷���
	Dom.SetLineColor(4);
	double dMaxJigan, dH;
	for(n=0; n<5; n++)
	{		
		dMaxJigan = m_dCalcJigan[n];
		dH = (bSM520 ? 0.04 : 0.035) * dMaxJigan + 0.7;			
		if(n==0) Dom.MoveTo(dH*dXScale, dMaxJigan);
		else	 Dom.LineTo(dH*dXScale, dMaxJigan);
	}

	// ���� ��ġ��
	Dom.SetLineColor(1);
	Dom.Circle(dCurH*dXScale, dCurJigan,0.5,TRUE);

	*pDom << Dom;
}
