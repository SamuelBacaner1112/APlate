// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutEtcDetail.h: interface for the CAPlateOutEtcDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTETCDETAIL_H__24086751_F226_41E5_8BA2_BEBDD15008AB__INCLUDED_)
#define AFX_APLATEOUTETCDETAIL_H__24086751_F226_41E5_8BA2_BEBDD15008AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutEtcDetail : public CDrawPage  
{
public:
	void DrawJackupDetail(CDomyun *pDom);
	void DrawJackupDanmyun(CDomyun *pDom);
	void TableSolePlateDetail(CDomyun *pDom);	
	void DrawDetailShearConnect(CDomyun *pDom);
	void DrawVStiffDetail1(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bVStiff = TRUE, BOOL bWeld = FALSE);
	void InsertDomyun(CDomyun *pDom, long nSubPage);

	void AddVStiffDetail1(CDomyun *pDom);
	void NoteVStiff(CDomyun *pDom);
	void TableVStiff(CDomyun *pDom);
	void PageTable(CDomyun *pDom);
	void NoteHStiffSp(CDomyun *pDom);
	void NoteHStiffV(CDomyun *pDom);
	void AddHStiffVDetail(CDomyun *pDom);
	void DrawHStiffVDetail(CDomyun *pDom, CPlateBasicIndex *pBx=NULL);
	void AddHStiffSpDetail(CDomyun *pDom);
	void DrawHStiffSpDetail(CDomyun *pDom, CPlateBasicIndex *pBx=NULL);
	void AddTaperDetail(CDomyun *pDom);
	void DrawTaperDetail(CDomyun *pDom, CPlateBasicIndex *pBx=NULL);
	void NoteScallop(CDomyun *pDom);
	void TableScallop(CDomyun *pDom);
	void AddScallopDetail(CDomyun *pDom);
	void DrawScallopDetail(CDomyun *pDom, CPlateBasicIndex *pBx=NULL);
	void DimTextNote(CDomyun *pDom);
	void DimTextWeldNote1(CDomyun *pDom);
	void DimTextWeldNote2(CDomyun *pDom);
	void DimTextWeldThick(CDomyun *pDom);
	CAPlateOutEtcDetail();
	virtual ~CAPlateOutEtcDetail();

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();

private:
	void ReplaceData(CDomyun *pDom, long nSubPage);
};

#endif // !defined(AFX_APLATEOUTETCDETAIL_H__24086751_F226_41E5_8BA2_BEBDD15008AB__INCLUDED_)
