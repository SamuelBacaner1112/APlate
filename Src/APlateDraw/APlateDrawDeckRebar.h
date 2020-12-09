// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawDeckRebar.h: interface for the CAPlateDrawDeckRebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWDECKREBAR_H__90B8CD9F_4F9B_4904_9F0F_9122DE8608B7__INCLUDED_)
#define AFX_APLATEDRAWDECKREBAR_H__90B8CD9F_4F9B_4904_9F0F_9122DE8608B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateDrawDeckRebar : public CDrawPage  
{
	friend class CARoadDrawSlabGen;
	
public:
	CAPlateDrawDeckRebar();
	virtual ~CAPlateDrawDeckRebar();

public:
	void DrawOnePage(); 
	long GetPageSu();
	CString GetTitle();
	double GetScale();

private:
	CPlateBasicIndex* m_pBxStt;
	CPlateBasicIndex* m_pBxEnd;
	long m_nG;
	CDRect m_rectDgk;
	
	long m_nArrayPageNum[10];		
	
private:
	CPlateBasicIndex* GetBxSttPage(long nSubPage, long nG, BOOL bSttBridge = TRUE);
	CPlateBasicIndex* GetBxEndPage(long nSubPage, long nG, BOOL bEndBridge = TRUE);
	CPlateBasicIndex* GetBxDimensionTable(long nPage);		

	BOOL IsStringerInPage(long nPage);
	BOOL IsStringerInPage(long nPage, long nG);

	void DrawSlabByPage(CDomyun *pDomP, long nSubPage);	
	void DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDan);
	void ReplaceData(CDomyun *pDom, CString szType);	
	
public:
	void AddSangseETC(CDomyun *pDom, long nIndex);
	void AddFrontGuardRebar(CDomyun* pDom);
	void AddPageGuardRebar(CDomyun* pDom);
	void AddGuardTable(CDomyun* pDom);
	void AddGuardDetail(CDomyun* pDom);
	void AddPlan(CDomyun *pDom, long nSubPage);	
	void AddJong(CDomyun *pDom, long nSubPage);
	CString AddFront(CDomyun *pDomP, long nSubPage, BOOL bLeft);
	void AddFrontDetail(CDomyun *pDomP, long nSubPage);
	void AddKeyPlan(CDomyun *pDomP, long nSubPage);

	void AddDimPlan(CDomyun *pDom, long nSubPage);
	void AddMainRebar(CDomyun *pDom, long nCycle, long nSection, long nSubPage);
	void AddDistriRebar(CDomyun *pDom, long nCycle, long nSubPage);
	void AddPageBM(CDomyun *pDomP);
	void AddRebarDetail(CDomyun *pDomP);
	CDPoint AddRebarTable(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle=_T(""), long nIdxTable=0, CDPoint xyBase=CDPoint(0, 0));
	CDPoint AddRebarTableGuard(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitlee=_T(""), long nIdxTable=0, CDPoint xyBase=CDPoint(0, 0));
	void AddDetailDanbu(CDomyun *pDom, BOOL bStt);
	void AddDeadLoadMomentDiagram(CDomyun *pDom, CDoubleArray &dArrX);

	double Bogan(double x1, double x2, double y1, double y2, double x);
	void MakeRebarList(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar);
	void MakeRebarListGuard(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar);
	void DimTextMarkDirection(CDomyun *pDomP, CString str, BOOL bFront);
};

#endif // !defined(AFX_APLATEDRAWDECKREBAR_H__90B8CD9F_4F9B_4904_9F0F_9122DE8608B7__INCLUDED_)
