// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawDBFrontRebar.h: interface for the CDrawDBFrontRebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDBFRONTREBAR_H__6AA7FDC8_192C_4FB6_8207_BCFA109EDF27__INCLUDED_)
#define AFX_DRAWDBFRONTREBAR_H__6AA7FDC8_192C_4FB6_8207_BCFA109EDF27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;

class AFX_EXT_CLASS CDrawDBFrontRebar  
{
public:
	CDrawDBFrontRebar(CDataManage *pDataMng);
	virtual ~CDrawDBFrontRebar();

	CDataManage *m_pDataMng;

public:
	// 철근
	void DrawMainRebar(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, BOOL bDrawJoint, long nShape=-1);
	void DrawMainRebarUp(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, BOOL bDrawJoint, long nShape=-1);
	void DrawMainRebarLo(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, BOOL bDrawJoint, long nShape=-1);
	void DrawMainRebarShear(CDomyun *pDom, long nSection);
	void DrawBeryukRebar(CDomyun *pDom, long nSection);
	void DrawSangseCrossBeam(CDomyun *pDom);	// 상세도는 일반부 위치에서 생성함.
	BOOL DrawSangseStringer(CDomyun *pDom);
	void DrawSangseOutBracket(CDomyun *pDom);
	// 치수
	void DimMainRebarJolib(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, double dTextRatio=1.0);
	void DimMainRebarUp(CDomyun *pDom, long nSection, double dTextRatio=1.0, BOOL bDimRangeGir=TRUE, BOOL bDimRangeCant=TRUE);
	void DimMainRebarLo(CDomyun *pDom, long nSection, BOOL bInput, double dTextRatio=1.0);
	void DimMainRebarLeft(CDomyun *pDom, long nSection, BOOL bInput, double dTextRatio=1.0);
	void DimMainRebarRigh(CDomyun *pDom, long nSection, BOOL bInput, double dTextRatio=1.0);
	// 마크
	void DrawMarkMainRebar(CDomyun *pDom, long nSection, long nCycle, BOOL bMirrorPos=FALSE);
	void DrawMarkBeryukRebar(CDomyun *pDom, long nSection, BOOL bMirrorPos=FALSE);
	void DrawMarkShearRebar(CDomyun *pDom, long nSection, BOOL bMirrorPos=FALSE);
};

#endif // !defined(AFX_DRAWDBFRONTREBAR_H__6AA7FDC8_192C_4FB6_8207_BCFA109EDF27__INCLUDED_)
