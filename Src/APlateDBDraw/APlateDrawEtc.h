// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawEtc.h: interface for the CAPlateDrawEtc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWETC_H__EB2D7AAA_7F12_4D06_B4AF_602FCAA4E5DB__INCLUDED_)
#define AFX_APLATEDRAWETC_H__EB2D7AAA_7F12_4D06_B4AF_602FCAA4E5DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPlateBasicIndex;
class AFX_EXT_CLASS CAPlateDrawEtc
{
public:
	CAPlateDrawEtc(CDataManage *pDataMng);
	virtual ~CAPlateDrawEtc();
private:
	CDataManage *m_pDataMng;
public:
	void DrawInputSlabAnchor(CDomyun *pDom);
	void DrawSlabAnchorFront(CDomyun *pDom);
	void DimSlabAnchorFront(CDomyun *pDom);
	void DrawSlabAnchorPlan(CDomyun *pDom);
	void DimSlabAnchorPlan(CDomyun *pDom);
	//상세
	void AddAnchorBarDetailCrBeam(CDomyun *pDom, CPlateBasicIndex *pBx, double Scale, double AnchorDia);
	void AddAnchorBarDetailVBracing(CDomyun *pDom, CPlateBasicIndex *pBx, double Scale, double AnchorDia);
	void DrawAnchorBarDetailPlan(CDomyun *pDomP, CPlateBasicIndex *pBx,
									double BX, double BY,
									double Scale, double AnchorDia);
	void DrawAnchorBarDetailSide(CDomyun *pDomP, CPlateBasicIndex *pBx,
									double BX, double BY,
									double Scale, double AnchorDia);
	// bFull : 평면도에서 복부판 필러에 사용할경우
	void DrawFillerSpliceRect(CDomyun *pDom, CPlateBasicIndex *pBx, __int64 nFlag, CPlatePieceObject *pObj, BOOL bFull=FALSE);

};

#endif // !defined(AFX_APLATEDRAWETC_H__EB2D7AAA_7F12_4D06_B4AF_602FCAA4E5DB__INCLUDED_)
