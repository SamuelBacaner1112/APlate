// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutPaintMap.h: interface for the CAPlateOutPaintMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTPAINTMAP_H__2B4CFEA2_7F78_43CE_842F_E35AA3750994__INCLUDED_)
#define AFX_APLATEOUTPAINTMAP_H__2B4CFEA2_7F78_43CE_842F_E35AA3750994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutPaintMap : public CDrawPage  
{
public:
	CAPlateOutPaintMap();
	virtual ~CAPlateOutPaintMap();

public:
	void InsertDomyun(CDomyun *pDom, long nSubPage);
	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();
private:

	void AddGeneral(CDomyun *pDom);
	void AddDanmyun(CDomyun *pDom);
	void AddSplice(CDomyun *pDom);
	void AddCrossBeam(CDomyun *pDom);


	void DrawGeneral(CDomyun *pDom, CPlateBasicIndex *pBx);	//일반도 그리기..
	void DrawSection(CDomyun *pDom, CPlateBasicIndex *pBx, long nMode);
	void DimTextNoteGen(CDomyun *pDom, CDPoint Base, long nMode);		//일반도 노트..
	void DrawTableGen(CDomyun *pDom);						//일반도 테이블...
	void DrawTableSec(CDomyun *pDom);
	void DrawTableSplice(CDomyun *pDom);
	void DrawTableCROSSBEAM(CDomyun *pDom);

	void DrawFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper); //현장이음 상하판..그리기...
	void DrawWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx); //현장이음 복부판..

	void DrawCrossBeamPlan(CDomyun *pDom, CPlateBasicIndex *pBx);//가로보 평면도..
	void DrawCrossBeamFront(CDomyun *pDom, CPlateBasicIndex *pBx);//가로보 정면도..

	void ReplaceData(CDomyun *pDom);
};

#endif // !defined(AFX_APLATEOUTPAINTMAP_H__2B4CFEA2_7F78_43CE_842F_E35AA3750994__INCLUDED_)
