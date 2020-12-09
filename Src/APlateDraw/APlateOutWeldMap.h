// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

 // APlateOutWeldMap.h: interface for the CAPlateOutWeldMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTWELDMAP_H__5F836677_D05B_4FC9_8132_B77BD6896C10__INCLUDED_)
#define AFX_APLATEOUTWELDMAP_H__5F836677_D05B_4FC9_8132_B77BD6896C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutWeldMap : public CDrawPage  
{
public:
	CAPlateOutWeldMap();
	virtual ~CAPlateOutWeldMap();

public:
 	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();

protected:
	CPlateBasicIndex* GetBxSttPage(long nSubPage);
	CPlateBasicIndex* GetBxEndPage(long nSubPage);
	void AddJong(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddDanmyun(CDomyun *pDom, double xDis, double yDis);
	void AddKeyPan(CDomyun *pDom, double xDis, double yDis, long nSubPage);
	void AddMarkSystem(CDomyun *pDom, double xDis, double yDis);
	long GetPageIndex(long nSubPage, long nBlock, CPlateBasicIndex *&pBxStt, CPlateBasicIndex *&pBxEnd, BOOL bKeyPlan=FALSE);
	void DimJongAllWeldMap(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDir, BOOL bUpper);
	double GetDisWebBySta(long nG, double SttSta, double EndSta, long nDir);
	void DrawCuttingMarkJong(CDomyun *pDom, CPlateBasicIndex *pBxStt,CPlateBasicIndex *pBxEnd,long nDir,BOOL bUpper);
	void DrawCuttingLineAndText(CDomyun *pDom, CPlateBasicIndex *pBx, CDPoint &StaCur, double y, BOOL bUpper);
};

#endif // !defined(AFX_APLATEOUTWELDMAP_H__5F836677_D05B_4FC9_8132_B77BD6896C10__INCLUDED_)
