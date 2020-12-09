// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PieceData.h: interface for the CPlatePieceData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEPIECEDATA_H__847A1262_4382_4A16_98C2_196D00CA6A94__INCLUDED_)
#define AFX_PLATEPIECEDATA_H__847A1262_4382_4A16_98C2_196D00CA6A94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlatePiecePoint.h"	// Added by ClassView

#define COL_JRIBBYWIDTH 18
#define ROW_JRIBBYWIDTH 15

#define COL_HRIBBYNUM	1
#define ROW_HRIBBYNUM	16

#define COL_JRIBBYNUM	1
#define ROW_JRIBBYNUM	21

#define COL_WEBWIDTH	1
#define ROW_WEBWIDTH	21

#define COL_WEBBYVSTIFF	2
#define ROW_WEBBYVSTIFF	21

#define COL_JRIBCUT		7
#define ROW_JRIBCUT		5

class AFX_EXT_CLASS CPlatePieceData : public CObject  
{
public:
	CPlatePieceData();
	virtual ~CPlatePieceData();
public:
	void	SetLByJRibWidth(double dThick, double dWidth, double dValue);
	double	GetLByJRibWidth(double dThick, double dWidth);	
	double	GetLMatByJRibWidth(long nRow, long nCol) { return m_JRibWidth(nRow,nCol); }
	void	SetLMatByJRibWidth(long nRow, long nCol, double dValue) { m_JRibWidth(nRow,nCol) = dValue; }
	//
	void	SetLByHRibNum(double dThick, double dValue);
	double	GetLByHRibNum(double dThick);	
	double	GetLMatByHRibNum(long nRow, long nCol) { return m_HRibNum(nRow,nCol); }
	void	SetLMatByHRibNum(long nRow, long nCol, double dValue) { m_HRibNum(nRow,nCol) = dValue; }
	//
	void	SetHByJRibNum(double dThick, double dValue);
	double	GetHByJRibNum(double dThick);	
	double	GetHMatByJRibNum(long nRow, long nCol) { return m_JRibNum(nRow,nCol); }
	void	SetHMatByByJRibNum(long nRow, long nCol, double dValue) { m_JRibNum(nRow,nCol) = dValue; }
	//
	void	SetWebLByThick(double dThick, double dValue);
	double	GetWebLByThick(double dThick);	
	double	GetWebMatByThick(long nRow, long nCol) { return m_WebThick(nRow,nCol); }
	void	SetWebMatByThick(long nRow, long nCol, double dValue) { m_WebThick(nRow,nCol) = dValue; }
	//
	void	SetWebByVStiff(double dThick, BOOL bHStiff, double dValue);
	double	GetWebByVStiff(double dThick, BOOL bHStiff);	
	double	GetWebMatByVStiff(long nRow, long nCol) { return m_WebByVStiff(nRow,nCol); }
	void	SetWebMatByVStiff(long nRow, long nCol, double dValue) { m_WebByVStiff(nRow,nCol) = dValue; }
	//
	void	SetJRibCut(long nThick, long nWidth, double dValue);
	double	GetJRibCut(long nThick, long nWidth);	
	double	GetJRibCutMat(long nRow, long nCol) { return m_JRibCut(nRow,nCol); }
	void	SetJRibCutMat(long nRow, long nCol, double dValue) { m_JRibCut(nRow,nCol) = dValue; }
	//
	void	Serialize(CArchive& ar);
public:
	void SetDefaultLByJRibWidth();
	void SetDefaultLByHRibNum();
	void SetDefaultHByJRibNum();
	void SetDefaultWebWidth();
	void SetDefaultWebByVStiff();
	void SetDefaultJRibCut();

public:
	P_TYPE GetPTypeByScallop(long nScallopType);
	double m_dBendFoldValue;
	double m_dButtWeldValue;
	double m_dJRibConnectValue;
	//
	// 다이아프램
	double m_dDiapAssmByHeight;
	double m_dDiapAssmByWidth;
	double m_dDiapWeldByHeight;
	double m_dDiapWeldByWidth;
	double m_dDiapWeldMax;
	double m_dDiapRingPlateAssm;
	// 단면
	double m_dDanCompByHRib;
	double m_dDanCompByTRib;
	double m_dDanTensByHRib;
	double m_dDanTensByTRib;
	double m_dDanVStiff;
	// 가로보
	double m_dCrossWeld;
	double m_dCrossWebWeldByWidth;
	double m_dCrossFlangeWeldByWidth;
	double m_dCrossWebAssmByWeld;
	double m_dCrossWebGassetAssmByHeight;
	double m_dCrossGassetAssmByHeight;

	// 세로보
	double m_dStringerWeld;
	double m_dStringerWebWeldByWidth;
	double m_dStringerFlangeWeldByWidth;
	double m_dStringerWebAssmByWeld;
	double m_dStringerWebGassetAssmByHeight;
	double m_dStringerGassetAssmByHeight;

	// 외측 가로보
	double m_dWingBeamWeld;
	double m_dWingBeamWebWeldByWidth;
	double m_dWingBeamFlangeWeldByWidth;
	double m_dWingBeamWebAssmByWeld;
	double m_dWingBeamWebGassetAssmByHeight;
	double m_dWingBeamGassetAssmByHeight;

	// 외측빔
	double m_dEndBeamWeld;
	double m_dEndBeamWebWeldByWidth;
	double m_dEndBeamFlangeWeldByWidth;
	double m_dEndBeamWebAssmByWeld;
	double m_dEndBeamWebGassetAssmByHeight;
	double m_dEndBeamGassetAssmByHeight;

	// 솔플레이트
	double m_dAssmByLong;
	double m_dAssmByThick;

private:
	CMatrix m_JRibWidth;
	CMatrix m_HRibNum;
	CMatrix m_JRibNum;	
	CMatrix m_WebThick;
	CMatrix m_WebByVStiff;
	CMatrix m_JRibCut;	
};

#endif // !defined(AFX_PLATEPIECEDATA_H__847A1262_4382_4A16_98C2_196D00CA6A94__INCLUDED_)
