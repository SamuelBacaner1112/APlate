// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutPlanGen.h: interface for the CAPlateOutPlanGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTPLANGEN_H__2F4C4668_8069_40CD_9A8D_1A862210FFD1__INCLUDED_)
#define AFX_APLATEOUTPLANGEN_H__2F4C4668_8069_40CD_9A8D_1A862210FFD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutPlanGen : public CDrawPage  
{
public:
	CAPlateOutPlanGen();
	virtual ~CAPlateOutPlanGen();

 	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();
	
	CDPointArray m_JweonInfoStaArr;		//x : 시작 스테이션 y: 끝 스테이션	

protected:
	CPlateBasicIndex* GetPageBxStt(long nG);
	CPlateBasicIndex* GetPageBxEnd(long nG);

	void AddPlan(CDomyun *pDom, long nSubPage);
	void DimTextMarkPlan(CDomyun *pDomP, long nSubPage);	
	void DimTextPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper);
	void DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDanUp, long nDanLow);
	void DimTextPyungFlange(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi);
	void DimTextPyungVStiff(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi);
	BOOL CheckStation(double sta1, double sta2);	
	void TextOutJewonAndMat(CDomyun *pDomP, CDPoint xy, CString strBuje, CString strMat, BOOL bDirForward=TRUE);
};

#endif // !defined(AFX_APLATEOUTPLANGEN_H__2F4C4668_8069_40CD_9A8D_1A862210FFD1__INCLUDED_)
