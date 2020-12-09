// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawDBBaseRebarPlan.h: interface for the DrawDBBaseRebarPlan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDBBASEREBARPLAN_H__5A72A2CD_1D93_44A8_9E93_0F06DB3E34B8__INCLUDED_)
#define AFX_DRAWDBBASEREBARPLAN_H__5A72A2CD_1D93_44A8_9E93_0F06DB3E34B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CDataManage;
class CPlateBasicIndex;
class CPlateGirderApp;
class CTwinVectorArray;

class AFX_EXT_CLASS CDrawDBBaseRebarPlan
{
public:
	CDrawDBBaseRebarPlan(CDataManage *pDataMng);
	virtual ~CDrawDBBaseRebarPlan();

private:
	CDataManage *m_pDataMng;  

protected:
	BOOL IsLeftOfJijumLine(CPlateBasicIndex *pBx, CDPoint ptStt, CDPoint ptEnd, BOOL bStt);	

public:
	// 주철근 그리기
	void DrawMainRebar(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nDrawSide=DRAW_SIDEALL, BOOL bDrawAlways = FALSE, BOOL bDrawJoint=FALSE);
	// 신축이음부 아래 주철근 그리기
	void DrawMainRebarUnderSinchuk(CDomyun *pDom, BOOL bStt, long nDrawSide=DRAW_SIDEALL, BOOL bDrawAlways = FALSE);
	// 사보강부 주철근  그리기
	void DrawMainRebarAtSkew(CDomyun *pDom, BOOL bUpper, long nDrawSide=DRAW_SIDEALL, BOOL bStt=TRUE, BOOL bEnd=TRUE, BOOL bDrawAlways = FALSE, BOOL bTrimRebar = FALSE);
	// 사보강철근 그리기
	void DrawSabogangRebar(CDomyun *pDom, BOOL bUpper, long nDrawSide=DRAW_SIDEALL, BOOL bStt=TRUE, BOOL bEnd=TRUE, BOOL bDrawAlways = FALSE);
	// 배력철근 그리기(일반부)
	void DrawBeryukRebarOnCenter(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nDrawSide=DRAW_SIDEALL);
	// 배력철근 그리기(중간지점부)
	void DrawBeryukRebarOnJijum(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nDrawSide=DRAW_SIDEALL);
	// 배력철근 그리기(신축이음부 하단)
	void DrawBeryukRebarUnderSinchuk(CDomyun *pDom, BOOL bSttSlab, long nDrawSide=DRAW_SIDEALL);
	// 가로보 보강 철근 그리기(BX)
	void DrawBogangRebarAtCrossBeamBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bDrawAlways = FALSE);
	// 스트링거 헌치하단 보강 철근 그리기(BX)
	void DrawBogangRebarAtStringerBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bDrawAlways = FALSE);
	void DrawBogangRebarAtStringer(CDomyun *pDom, long nStg, long nSide, BOOL bDrawAlways = FALSE);
	// 외측가로보 보강 철근 그리기(BX)
	void DrawBogangRebarAtOutBrkBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft, long nSide, BOOL bDrawAlways = FALSE);
	void DrawBogangRebarAtOutBrk(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	// 가로보 헌치하단 보강 철근 그리기(BXStt~BxEnd)
	void DrawBogangRebarAtCrossBeam(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	// 스터드보강철근 그리기(단부)
	void DrawStudBogangRebarAtDanbu(CDomyun *pDom, long nG, BOOL bLeft, BOOL bSttSlab);
	// 스터드보강철근 그리기(중간지점)
	void DrawStudBogangRebarAtCenJijum(CDomyun *pDom, long nG, BOOL bLeft, long nJigan, BOOL bSttJigan);
	// 스터드보강철근 그리기(지간별로)
	void DrawStudBogangRebarByJigan(CDomyun *pDom, long nJigan);
	// Trim 관련
	BOOL IsDrawMainRebarArea(CDPoint pt1, CDPoint pt2);
	void DrawSangseUnderSinchuk(CDomyun *pDom, BOOL bSttSlab, long nDrawSide);
	void DrawSangseStudBagang(CDomyun *pDom, long nJigan);

	void DrawTrimLine(CDomyun *pDom, CPtrArray &ObArcArr, CPtrArray &ObDeleteCircleArc, CPtrArray &ObCircleArrCB, CPtrArray &ObCircleArrST, CPtrArray &ObCircleArrOBK, CPtrArray &ObCircleArrDan, CPtrArray &ObCircleArrCant, CDPointArray &xyArrCB, CDPointArray &xyArrST, CDPointArray &xyArrOBK, CVectorArray &xyArrDan, CVectorArray &xyArrCant, CPlateBasicIndex *pBxStt=NULL, CPlateBasicIndex *pBxEnd=NULL);
	void RemoveByLineFence(CDomyun *pDom, long nIndex);
	BOOL IsInDrawPostion(double dStaStt, double dStaEnd);
	void TrimByJijumBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bStt);
	
	// 마크관련
	void DrawMark(CDomyun *pDom, long nJigan);
	void DrawMarkMainDiv(CDomyun *pDom, long nJigan, BOOL bUpper);
	void DrawMarkMainNoDiv(CDomyun *pDom, long nJigan, BOOL bUpper);
	void DrawMarkMainLowerEtc(CDomyun *pDom, long nJigan);
	void DrawMarkMainSkew(CDomyun *pDom, long nJigan, BOOL bUpper);
	void DrawMarkBeryuk(CDomyun *pDom, long nJigan);
	void DrawMarkCB_ST_OBK(CDomyun *pDom, long nJigan);
	void DrawMarkSangseDanbu(CDomyun *pDom, long nJigan);
	void DrawMarkSangseCant(CDomyun *pDom, long nJigan);
	void DrawRebarMark(CDomyun *pDom, CDPointArray xyArr, CString szMark, CString szDia, long nDir, BOOL bLeft, double dExtendL=0, long nArrIdx=-1, BOOL bReverse=FALSE);
	void DrawRebarMark2(CDomyun *pDom, CDPointArray xyArr, CDPointArray xyArrTot, CString szMark, CString szDia, long nDir, BOOL bLeft, double dExtendL=0, long nArrIdx=-1, BOOL bReverse=FALSE);
	void DrawRebarMarkByAngle(CDomyun *pDom, CDPointArray xyArr, CString szMark, CString szDia, CDPoint vAng, long nDir, double dExtendL=0, long nArrIdx=-1, BOOL bReverse=FALSE);
	void DrawRebarMarkLine(CDomyun *pDom, CDPointArray xyArr);

	void DimBogangRebarAtCrossBeamBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bDrawAlways);
	void DimBogangRebarAtStringer(CDomyun *pDom, long nStg, long nSide, BOOL bDrawAlways);
	void DimBogangRebarAtOutBracketBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft, long nSide, BOOL bDrawAlways);

	void DrawJoint(CDomyun *pDom, vector<REBAR_PLAN> *pvPlan, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bMainRebar);

	
};

#endif // !defined(AFX_DRAWDBBASEREBARPLAN_H__5A72A2CD_1D93_44A8_9E93_0F06DB3E34B8__INCLUDED_)
