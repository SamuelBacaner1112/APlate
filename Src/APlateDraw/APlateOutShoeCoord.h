// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutShoeCoord.h: interface for the CAPlateOutShoeCoord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTSHOECOORD_H__3CD9A634_CF21_43E6_835E_86881791644D__INCLUDED_)
#define AFX_APLATEOUTSHOECOORD_H__3CD9A634_CF21_43E6_835E_86881791644D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutShoeCoord : public CDrawPage  
{
public:
	CAPlateOutShoeCoord();
	virtual ~CAPlateOutShoeCoord();

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();

	CDRect DgkBound;	
protected:
	void AddPageElevationCalc();
	void AddPageShoeCoordinate();
	void AddTableShoeLegend(CDomyun *pDomP, CDPoint Base);
	void AddTableShoeList(CDomyun *pDomP, CDPoint Base);
	void AddTableShoeCoordinate(CDomyun *pDomP, CDPoint Base, long nTableSu);
	void AddElevationCalcTable(CDomyun *pDomP);
	void AddElevationFrontSection(CDomyun *pDomP);
	void AddElevationShoeDetail(CDomyun *pDomP);
	void AddShoeCoordinate(CDomyun *pDomP);
	void DimShoeMortarDetail(CDomyun *pDomP, long nPier, long nG);
};

#endif // !defined(AFX_APLATEOUTSHOECOORD_H__3CD9A634_CF21_43E6_835E_86881791644D__INCLUDED_)
