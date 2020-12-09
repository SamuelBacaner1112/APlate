// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawCalc.h: interface for the CAPlateDrawCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWCALC_H__4C91433A_2856_4C03_9008_8EAA18526784__INCLUDED_)
#define AFX_APLATEDRAWCALC_H__4C91433A_2856_4C03_9008_8EAA18526784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateCalcStd;
class AFX_EXT_CLASS CAPlateDrawCalc  
{
public:
	CAPlateDrawCalc(CDataManage *pDataMng);
	virtual ~CAPlateDrawCalc();

public:
	CDataManage *m_pDataManage;

public:
	void DrawGuardWall(CDomyun *pDom, CCentSeparation *pGWall, double BX, double BY);
	void DrawGuardWallInputLeft(CDomyun *pDom, CCentSeparation* pSep, double LEN);
//	void DrawGuardInputLeftDim(CDomyun *pDom, CCentSeparation* pSep, double LEN);
	void DrawGuardWallInputRight(CDomyun *pDom, CCentSeparation* pSep, double LEN);
//	void DrawGuardInputRightDim(CDomyun *pDom, CCentSeparation* pSep, double LEN);
	void DrawGuardDivideDimLeft(CDomyun *pDom, CCentSeparation* pSep, double LEN, BOOL bExcel=FALSE);
	void DrawGuardDivideDimRight(CDomyun *pDom, CCentSeparation* pSep, double LEN, BOOL bExcel=FALSE);

	void DrawFloorLoadInputLeftDim(CDomyun *pDom, CCentSeparation* pSep, double LEN);
	void DrawFloorLoadInputRightDim(CDomyun *pDom, CCentSeparation* pSep, double LEN);

	void SetDBLoad(double dDBLoadCar, double dDBLoadTank, double dDBLoadTrailer);
	void SetDBLoadCar(double dDBLoadCar)			{m_dDBLoadCar		= dDBLoadCar;		};
	void SetDBLoadTank(double dDBLoadTank)			{m_dDBLoadTank		= dDBLoadTank;		};
	void SetDBLoadTrailer(double dDBLoadTrailer)	{m_dDBLoadTrailer	= dDBLoadTrailer;	};
	void SetGuardType(long nGuardType)				{m_nGuardType		= nGuardType;		};
	void AscendSort(CDoubleArray &DArrWheight, CStringArray &DArrHeight);

private:
	double	m_dDBLoadCar;
	double	m_dDBLoadTank;
	double	m_dDBLoadTrailer;
	long	m_nGuardType;

	void DrawLoadArrow(CDomyun *pDom, CDPoint xyUp, CDPoint xyDn);
};

#endif // !defined(AFX_APLATEDRAWCALC_H__4C91433A_2856_4C03_9008_8EAA18526784__INCLUDED_)
