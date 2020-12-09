// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawDBBaseRebarTU.h: interface for the CDrawDBBaseRebarTU class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDBBASEREBARTU_H__3510810E_3B1E_4FCF_A886_4F090A1DE5BD__INCLUDED_)
#define AFX_DRAWDBBASEREBARTU_H__3510810E_3B1E_4FCF_A886_4F090A1DE5BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPlateBasicIndex;
class CPlateGirderApp;
class CTwinVectorArray;

class AFX_EXT_CLASS CDrawDBBaseRebarTU  
{
public:
	CDrawDBBaseRebarTU(CDataManage *pDataMng);
	virtual ~CDrawDBBaseRebarTU();

private:
	CDataManage *m_pDataMng;  

public:
	void DrawMainRebarTUAtPlan(CDomyun *pDom, long nG);
	void DimMainRebarTUAtPlan(CDomyun *pDom, long nG, BOOL bInput=FALSE);
	void DrawSabogangRebarTUAtPlan(CDomyun *pDom, long nG);
	void DimSabogangRebarTUAtPlan(CDomyun *pDom, long nG, BOOL bInput=FALSE);
	void DrawBeryukRebarAtPlan(CDomyun *pDom, long nG);
	void DrawHunchRebarTUAtPlan(CDomyun *pDom, long nG);

	void DrawBincConcRebarAtJong(CDomyun *pDom, long nG, BOOL bInput);
	void DimBincConcRebarAtJong(CDomyun *pDom, long nG, BOOL bInput=FALSE);
	void DrawBincConcRebarAtCross(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDrawAnchor);
	void DimBincConcRebarAtCross(CDomyun *pDom, long nG, BOOL bInput=TRUE, BOOL bAnchor=TRUE, BOOL bDanbuJewon=FALSE);

	void DrawUpDownLine(CDomyun *pDomP, long nG, BOOL bInput);
	void DrawMark(CDomyun *pDom, long nG, long nDrawPos, BOOL bAnchor=TRUE, BOOL bDanbuJewon=FALSE);
	void DrawMarkPlan(CDomyun *pDom, long nG);
	void DrawMarkJong(CDomyun *pDom, long nG);
	void DrawMarkCross(CDomyun *pDom, long nG, BOOL bAnchor, BOOL bDanbuJewon);

	void DrawTrimLine(CDomyun *pDom, long nG, long nType);
	void DrawTrimLineJong(CDomyun *pDom, long nG);
};

#endif // !defined(AFX_DRAWDBBASEREBARTU_H__3510810E_3B1E_4FCF_A886_4F090A1DE5BD__INCLUDED_)
