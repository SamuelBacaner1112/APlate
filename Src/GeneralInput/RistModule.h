// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RistModule.h: interface for the CRistModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RISTMODULE_H__D93C1123_AAC6_4141_89F1_41C1863208FD__INCLUDED_)
#define AFX_RISTMODULE_H__D93C1123_AAC6_4141_89F1_41C1863208FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRistModule  
{
public:
	CRistModule();
	virtual ~CRistModule();

public:
	static CString m_gFlangeWidthTitle[2][5];
	static double m_dCalcJigan[5];
	static double m_gFlangeWidthValue[2][2][5];
	static double m_gFlangeUpperThickValue[2][5][12];
	static double m_gFlangeLowerThickValue[2][5][12];
	static double m_gFlangeLengthValue[2][5][12];	
	static double m_gWebThickValue[2][5][12];

public:
	double Bogan(double x1, double x2, double y1, double y2, double x);
	long GetIndexNearByMaxJigan(double dMaxJiGan);
	long GetRealPos(long nQtyJigan, long nCurJigan, long nPos);

	double GetLengthMaxJigan(long nQtyJigan, double *dLengthArray);

	double GetHeightGirder(long nQtyJigan, double *dLengthArray, BOOL bSM520);
	long GetDividCount(double dMaxJiGan, long nQtyJigan, BOOL bSM520);
	double GetRatio(double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520);
	double GetFlangeThick(double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520, BOOL bUpper);
	double GetWebThick(double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520);
	double GetFlangeWidth(double dGirderH, double dMaxJiGan, long nQtyJigan, long nCurJigan, long nPos, BOOL bSM520, BOOL bUpper);
	void DrawRistGraph(CDomyun *pDom, double dCurJigan, double dCurH, BOOL bSM520);
};

#endif // !defined(AFX_RISTMODULE_H__D93C1123_AAC6_4141_89F1_41C1863208FD__INCLUDED_)
