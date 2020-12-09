// PieceData.cpp: implementation of the CPlatePieceData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlatePieceData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlatePieceData::CPlatePieceData()
{	
	m_dBendFoldValue = 0.15;
	m_dButtWeldValue = 0.5;
	m_dJRibConnectValue = 5;

	SetDefaultLByJRibWidth();
	SetDefaultLByHRibNum();
	SetDefaultHByJRibNum();
	SetDefaultWebWidth();
	SetDefaultWebByVStiff();
	SetDefaultJRibCut();

	// 다이아프램
	m_dDiapAssmByHeight	= 0.00;
	m_dDiapAssmByWidth	= -2.00;
	m_dDiapWeldByHeight	= 0.00;
	m_dDiapWeldByWidth	= 0.60;
	m_dDiapWeldMax		= 18.00;
	m_dDiapRingPlateAssm= -5.00;
	// 단면
	m_dDanCompByHRib	= -3.0;
	m_dDanCompByTRib	= -3.0;
	m_dDanTensByHRib	= -2.0;
	m_dDanTensByTRib	= -2.0;
	m_dDanVStiff		= -2.0;
	// 가로보
	m_dCrossWeld					= 18.00;
	m_dCrossWebWeldByWidth			= 0.60;
	m_dCrossFlangeWeldByWidth		= -1.00;
	m_dCrossWebAssmByWeld			= -2.00;
	m_dCrossWebGassetAssmByHeight	= 0.00;
	m_dCrossGassetAssmByHeight		= 0.00;

	// 세로보
	m_dStringerWeld						= 18.00;
	m_dStringerWebWeldByWidth			=  0.60;
	m_dStringerFlangeWeldByWidth		= -1.00;
	m_dStringerWebAssmByWeld			= -2.00;
	m_dStringerWebGassetAssmByHeight	=  0.00;
	m_dStringerGassetAssmByHeight		=  0.00;

	// 외측 가로보
	m_dWingBeamWeld						= 18.00;
	m_dWingBeamWebWeldByWidth			=  0.60;
	m_dWingBeamFlangeWeldByWidth		= -1.00;
	m_dWingBeamWebAssmByWeld			= -2.00;
	m_dWingBeamWebGassetAssmByHeight	=  0.00;
	m_dWingBeamGassetAssmByHeight		=  0.00;

	// 외측빔
	m_dEndBeamWeld						= 18.00;
	m_dEndBeamWebWeldByWidth			=  0.60;
	m_dEndBeamFlangeWeldByWidth			= -1.00;
	m_dEndBeamWebAssmByWeld				= -2.00;
	m_dEndBeamWebGassetAssmByHeight		=  0.00;
	m_dEndBeamGassetAssmByHeight		=  0.00;

	// 솔플레이트
	m_dAssmByLong					= 60.00;
	m_dAssmByThick					= 2.00;
}

CPlatePieceData::~CPlatePieceData()
{

}

void CPlatePieceData::SetDefaultLByJRibWidth()
{
	double Data[ROW_JRIBBYWIDTH][COL_JRIBBYWIDTH] = {
						{0.43,	0.41,	0.39,	0.37,	0.34,	0.32,	0.30,	0.28,	0.26,	0.24,	0.21,	0.19,	0.17,	0.15,	0.00,	0.00,	0.00,	0.00 },
						{0.41,	0.39,	0.36,	0.34,	0.32,	0.29,	0.27,	0.25,	0.22,	0.20,	0.18,	0.16,	0.13,	0.00,	0.00,	0.08,	0.05,	0.03 },
						{0.39,	0.36,	0.34,	0.32,	0.29,	0.27,	0.25,	0.23,	0.20,	0.18,	0.16,	0.13,	0.11,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.37,	0.34,	0.32,	0.30,	0.27,	0.25,	0.23,	0.20,	0.18,	0.16,	0.14,	0.11,	0.09,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.34,	0.32,	0.30,	0.27,	0.25,	0.23,	0.21,	0.18,	0.16,	0.14,	0.12,	0.09,	0.07,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.32,	0.30,	0.28,	0.25,	0.23,	0.18,	0.16,	0.14,	0.12,	0.09,	0.07,	0.05,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.30,	0.28,	0.25,	0.23,	0.21,	0.19,	0.16,	0.14,	0.12,	0.09,	0.07,	0.05,	0.02,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.28,	0.26,	0.23,	0.21,	0.19,	0.16,	0.14,	0.12,	0.09,	0.07,	0.05,	0.03,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.26,	0.23,	0.21,	0.19,	0.17,	0.14,	0.12,	0.10,	0.07,	0.05,	0.03,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.24,	0.21,	0.19,	0.17,	0.14,	0.12,	0.10,	0.07,	0.05,	0.03,	0.01,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.21,	0.19,	0.17,	0.15,	0.12,	0.10,	0.08,	0.05,	0.03,	0.01,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.19,	0.17,	0.15,	0.12,	0.10,	0.08,	0.05,	0.03,	0.01,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 },
						{0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00 }};

	m_JRibWidth.Resize(ROW_JRIBBYWIDTH,COL_JRIBBYWIDTH);
	for(long nRow=0;nRow<ROW_JRIBBYWIDTH;nRow++)
		for(long nCol=0;nCol<COL_JRIBBYWIDTH;nCol++)
			m_JRibWidth(nRow,nCol) = Data[nRow][nCol];
}

double CPlatePieceData::GetLByJRibWidth(double dThick, double dWidth)
{
	long nRow = (long)((dThick - 10) / 2);
	long nCol = (long)((dWidth - 300) / 50);
	return m_JRibWidth(nRow,nCol);
}

void CPlatePieceData::SetLByJRibWidth(double dThick, double dWidth, double dValue)
{
	long nRow = (long)((dThick - 10) / 2);
	long nCol = (long)((dWidth - 300) / 50);

	m_JRibWidth(nRow,nCol) = dValue;
}

// =======================================================================================================
void CPlatePieceData::SetDefaultLByHRibNum()
{
	double Data[ROW_JRIBBYNUM][COL_JRIBBYNUM] = {
		{0.16},
		{0.15},
		{0.14},
		{0.13},
		{0.12},
		{0.12},
		{0.11},
		{0.10},
		{0.09},
		{0.08},
		{0.07},
		{0.06},
		{0.00},
		{0.00},
		{0.00},
		{0.0}};		

	m_HRibNum.Resize(ROW_HRIBBYNUM,COL_HRIBBYNUM);
	for(long nRow=0;nRow<ROW_HRIBBYNUM;nRow++)
		for(long nCol=0;nCol<COL_HRIBBYNUM;nCol++)
			m_HRibNum(nRow,nCol) = Data[nRow][nCol];
}

double CPlatePieceData::GetLByHRibNum(double dThick)
{	
	long nRow = (long)(dThick - 10) / 2;
	return m_HRibNum(nRow,0);
}

void CPlatePieceData::SetLByHRibNum(double dThick, double dValue)
{
	long nRow = (long)(dThick - 10) / 2;

	m_HRibNum(nRow,0) = dValue;
}

// =======================================================================================================
void CPlatePieceData::SetDefaultHByJRibNum()
{
	double Data[ROW_JRIBBYNUM][COL_JRIBBYNUM] = {	
		{0.44},
		{0.39},
		{0.34},
		{0.30},
		{0.25},
		{0.20},
		{0.15},
		{0.10},};
		
	m_JRibNum.Resize(ROW_JRIBBYNUM,COL_JRIBBYNUM);
	for(long nRow=0;nRow<ROW_JRIBBYNUM;nRow++)
		for(long nCol=0;nCol<COL_JRIBBYNUM;nCol++)
			m_JRibNum(nRow,nCol) = Data[nRow][nCol];
}

double CPlatePieceData::GetHByJRibNum(double dThick)
{	
	long nRow = (long)(dThick - 10) / 2;
	return m_JRibNum(nRow,0);
}

void CPlatePieceData::SetHByJRibNum(double dThick, double dValue)
{
	long nRow = (long)(dThick - 10) / 2;

	m_JRibNum(nRow,0) = dValue;
}

// =======================================================================================================
void CPlatePieceData::SetDefaultWebWidth()
{
	double Data[ROW_WEBWIDTH][COL_WEBWIDTH] = {		
		{0.35},
		{0.31},
		{0.27},
		{0.23},
		{0.19},
		{0.15},
		};

	m_WebThick.Resize(ROW_WEBWIDTH,COL_WEBWIDTH);
	for(long nRow=0;nRow<ROW_WEBWIDTH;nRow++)
		for(long nCol=0;nCol<COL_WEBWIDTH;nCol++)
			m_WebThick(nRow,nCol) = Data[nRow][nCol];
}

double CPlatePieceData::GetWebLByThick(double dThick)
{	
	long nRow = (long)((dThick - 10) / 2);
	return m_WebThick(nRow,0);
}

void CPlatePieceData::SetWebLByThick(double dThick, double dValue)
{
	long nRow = (long)((dThick - 10) / 2);

	m_WebThick(nRow,0) = dValue;
}

// =======================================================================================================
void CPlatePieceData::SetDefaultWebByVStiff()
{
	double Data[ROW_WEBBYVSTIFF][COL_WEBBYVSTIFF] = {			
		{0.15,0.48},
		{0.14,0.42},
		{0.12,0.37},
		{0.11,0.31},
		{0.09,0.26},
		{0.08,0.20},
		};

	m_WebByVStiff.Resize(ROW_WEBBYVSTIFF,COL_WEBBYVSTIFF);
	for(long nRow=0;nRow<ROW_WEBBYVSTIFF;nRow++)
		for(long nCol=0;nCol<COL_WEBBYVSTIFF;nCol++)
			m_WebByVStiff(nRow,nCol) = Data[nRow][nCol];
}

double CPlatePieceData::GetWebByVStiff(double dThick, BOOL bHStiff)
{	
	long nRow = (long)((dThick - 10) / 2);
	return m_WebByVStiff(nRow,bHStiff ? 0 : 1);
}

void CPlatePieceData::SetWebByVStiff(double dThick, BOOL bHStiff, double dValue)
{
	long nRow = (long)((dThick - 10) / 2);

	m_WebByVStiff(nRow,bHStiff ? 0 : 1) = dValue;
}

// =======================================================================================================
void CPlatePieceData::SetDefaultJRibCut()
{
	double Data[ROW_JRIBCUT][COL_JRIBCUT] = {			
		{0.7,	0.0,	0.00,	0.00,	0.00,	0.00,	0.00},
		{0.6,	0.5,	0.45,	0.35,	0.3,	0.00,	0.00},
		{0.0,	0,		0.4,	0.00,	0,		0.00,	0.00},
		{0.0,	0,		0,		0.3,	0,		0.00,	0.00},
		{0.0,	0,		0,		0,		0,		0.00,	0.25}
		};

	m_JRibCut.Resize(ROW_JRIBCUT,COL_JRIBCUT);
	for(long nRow=0;nRow<ROW_JRIBCUT;nRow++)
		for(long nCol=0;nCol<COL_JRIBCUT;nCol++)
			m_JRibCut(nRow,nCol) = Data[nRow][nCol];
}

double CPlatePieceData::GetJRibCut(long nThick, long nWidth)
{	
	long nCol = (nThick - 10) / 2;	
	if(nWidth > 150) nWidth += 50;	// 임시 180 때문에 임시로 둠
	long nRow = (nWidth - 100) / 50;
	
	return m_JRibCut(nRow,nCol);
}

void CPlatePieceData::SetJRibCut(long nThick, long nWidth, double dValue)
{
	long nCol = (nThick - 10) / 2;
	if(nWidth > 150) nWidth += 50;	// 임시 180 때문에 임시로 둠
	long nRow = (nWidth - 100) / 50;

	m_JRibCut(nRow,nCol) = dValue;
}

///////////////////////////////////////////////////////////////////////////////
void CPlatePieceData::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		//
		ar << m_dBendFoldValue;
		ar << m_dButtWeldValue;
		ar << m_dJRibConnectValue;
		//
		// 다이아프램
		ar << m_dDiapAssmByHeight;
		ar << m_dDiapAssmByWidth;
		ar << m_dDiapWeldByHeight;
		ar << m_dDiapWeldByWidth;
		ar << m_dDiapWeldMax;
		ar << m_dDiapRingPlateAssm;
		// 단면
		ar << m_dDanCompByHRib;
		ar << m_dDanCompByTRib;
		ar << m_dDanTensByHRib;
		ar << m_dDanTensByTRib;
		ar << m_dDanVStiff;
		// 가로보
		ar << m_dCrossWeld;
		ar << m_dCrossWebWeldByWidth;
		ar << m_dCrossFlangeWeldByWidth;
		ar << m_dCrossWebAssmByWeld;
		ar << m_dCrossWebGassetAssmByHeight;
		ar << m_dCrossGassetAssmByHeight;

		// 세로보 
		ar << m_dStringerWeld;
		ar << m_dStringerWebWeldByWidth;
		ar << m_dStringerFlangeWeldByWidth;
		ar << m_dStringerWebAssmByWeld;
		ar << m_dStringerWebGassetAssmByHeight;
		ar << m_dStringerGassetAssmByHeight;

		// 솔플레이트
		ar << m_dAssmByLong;
		ar << m_dAssmByThick;
	}
	else
	{
		ar >> nFlag;

		//
		ar >> m_dBendFoldValue;
		ar >> m_dButtWeldValue;
		ar >> m_dJRibConnectValue;
		//
		// 다이아프램
		ar >> m_dDiapAssmByHeight;
		ar >> m_dDiapAssmByWidth;
		ar >> m_dDiapWeldByHeight;
		ar >> m_dDiapWeldByWidth;
		ar >> m_dDiapWeldMax;
		ar >> m_dDiapRingPlateAssm;
		// 단면
		ar >> m_dDanCompByHRib;
		ar >> m_dDanCompByTRib;
		ar >> m_dDanTensByHRib;
		ar >> m_dDanTensByTRib;
		ar >> m_dDanVStiff;
		// 가로보
		ar >> m_dCrossWeld;
		ar >> m_dCrossWebWeldByWidth;
		ar >> m_dCrossFlangeWeldByWidth;
		ar >> m_dCrossWebAssmByWeld;
		ar >> m_dCrossWebGassetAssmByHeight;
		ar >> m_dCrossGassetAssmByHeight;
			// 세로보 
		ar >> m_dStringerWeld;
		ar >> m_dStringerWebWeldByWidth;
		ar >> m_dStringerFlangeWeldByWidth;
		ar >> m_dStringerWebAssmByWeld;
		ar >> m_dStringerWebGassetAssmByHeight;
		ar >> m_dStringerGassetAssmByHeight;
		// 솔플레이트
		ar >> m_dAssmByLong;
		ar >> m_dAssmByThick;
	}
	m_JRibWidth.Serialize(ar);
	m_HRibNum.Serialize(ar);
	m_JRibNum.Serialize(ar);	
	m_WebThick.Serialize(ar);
	m_WebByVStiff.Serialize(ar);
	m_JRibCut.Serialize(ar);	
}

P_TYPE CPlatePieceData::GetPTypeByScallop(long nScallopType)
{
	switch (nScallopType)
	{
		case 0:
			return P_TYPE_SCALLOP;		
		case 1:
		case 2:
			return P_TYPE_CHAMFER;		
		case 3:
			return P_TYPE_LINE;		
	}
	return P_TYPE_LINE;
}
