// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutSplice.h: interface for the CAPlateOutSplice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTSPLICE_H__7C062554_1CF9_4D32_B5D9_7C4BDCDC4DE3__INCLUDED_)
#define AFX_APLATEOUTSPLICE_H__7C062554_1CF9_4D32_B5D9_7C4BDCDC4DE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOLT_DIA 15
#define BOLT_MAX 15

typedef CTypedPtrArray<CObArray,CPlateSplice*> CSpliceArray;

class CAPlateOutSplice : public CDrawPage  
{
public:
	CAPlateOutSplice();
	virtual ~CAPlateOutSplice();

public: 
	CObject* m_pOutShopStd;
	CPtrList m_BMList;	//¿Á∑·«• 
	CDRect	 m_DgkBound;
	BOOL     m_bNomyun;

public:
	void DrawOnePage();

public:
	void AddUpperSplice(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage);
	void AddUpperSpliceTable(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage);
	void AddWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage);
	void AddWebSpliceTable(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage);
	void AddLowerSplice(CDomyun *pDom,  CPlateBasicIndex *pBx, long nIdxOnPage);
	void AddLowerSpliceTable(CDomyun *pDom,  CPlateBasicIndex *pBx, long nIdxOnPage);

	void DrawWebSpliceBx(CDomyun* pDom, CPlateBasicIndex *pBx, BOOL bLeft);

	void DrawTopFlangeTable(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawBottomFlangeTable(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawSpliceTable(CDomyun *pDom, long nSubPage, double xDis, double yDis);
	void DrawWebTable(CDomyun *pDom, CPlateBasicIndex *pBx);

	void DimWebSpliceBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft);
	void AddMaterialTable(CDomyun *pDom, long nSubPage);

	long GetPageSu();
	CString GetTitle();
	void AddSpTiTle(CDomyun *pDom, long subPage, long nIdxOnPage);

private:
	long MakeSpliceList();
	long GetBoltSu(CPlateBasicIndex *pBx, long nUpDown);
	double GetScale();
	//
	CTypedPtrArray<CObArray,CPlateSplice*> m_SpList;
protected:
	CPlateBasicIndex* GetBxPage(long nSubPage, long nIdxOnPage);
	CString DimSpTitle(CDomyun *pDom, CPlateBasicIndex* pBx);
};

#endif // !defined(AFX_APLATEOUTSPLICE_H__7C062554_1CF9_4D32_B5D9_7C4BDCDC4DE3__INCLUDED_)
