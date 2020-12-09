// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawCross.h: interface for the CAPlateDrawCross class.
//  
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWCROSS_H__ACB96FA4_1701_44B6_BFF8_B198FBBD1036__INCLUDED_)
#define AFX_APLATEDRAWCROSS_H__ACB96FA4_1701_44B6_BFF8_B198FBBD1036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPlateBasicIndex;
class CPlateGirderApp;
class CAPlateDrawStd;

class AFX_EXT_CLASS CAPlateDrawCross 
{
public: 
	CAPlateDrawCross(CDataManage *pDataMng);
	virtual ~CAPlateDrawCross();  

private:
	CDataManage *m_pDataMng;
public:
	enum { H, B, T1, T2, R1, R2 };
	enum { UPPER, LOWER, ALL, NOTDRAWVSTIFF};
//	enum { UP, DOWN, CENTER };
//function
public:
	void DrawFrontSplicePlate_FlangeBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft);
	void DrawFrontSplicePlate_ShearBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft);
	void DrawFrontSplicePlate_FillerBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft);	
	void DrawFrontSplicePlate_MomentBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bUpper, BOOL bDrawBolt);
	void DstParallelogramXyBottom(CDomyun *pDomP, CDPoint xyStt, CDPoint xyEnd, double dHeight);
	void DstParallelogramHorz(CDomyun *pDomP, double Length, double Width, double slop ,double BX, double BY);
	void DstParallelogramVert(CDomyun *pDomP, double Height, double Thick, double slop ,double BX, double BY);
	void DstDrawBoltHole(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
					double dD, long nE, double dF,
					double BoltSize, double SlopStt=0, double SlopEnd=0, double dF1=0,
					double RadRotate=0, BOOL bLeft=TRUE, BOOL bRight=TRUE, long nBoltArrange=0, CDPoint xy=CDPoint(0,0));
	void DstDrawBoltLine(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
					double dD, long nE, double dF,
					double BoltSize, double SlopStt=0, double SlopEnd=0, double dF1=0,
					double RadRotate=0, BOOL bLeft=TRUE, BOOL bRight=TRUE, long nBoltArrange=0, CDPoint xy=CDPoint(0,0));
	void DstDrawGassetSupPlate(CDomyun *pDom, CPlateBasicIndex *pBx, double Len, double Thick, BOOL bLeft);

public:  
	void DrawFrontHGussetSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim);
	void DrawFrontHGussetSplice(CDomyun *pDom, CPlateBasicIndex *pBx, CDPoint xyU, double dHeight, double dWidth, BOOL bLeft);
	void DrawHStiffFront(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft=TRUE);
	//V Bracing Type
	void DrawInputVBracingFront(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawInputVBracingPlan(CDomyun *pDom, CPlateBasicIndex *pBx);

	void DrawVBracingBeamOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon);
	void DrawVBracingBeamSangSeOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSangNo);
	void DrawVBracingAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx);
	//수직브레이싱 이음판
	void DrawVBracingSpliceOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim);
	void DrawVBracingSpliceSangseOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim, long nDetailNum);
	void DrawVBracingSpliceBoltLT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe=FALSE, BOOL bDim=FALSE);
	void DrawVBracingSpliceBoltLD(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe=FALSE, BOOL bDim=FALSE);
	void DrawVBracingSpliceBoltRT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe=FALSE, BOOL bDim=FALSE);
	void DrawVBracingSpliceBoltRD(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe=FALSE, BOOL bDim=FALSE);
	
	void DimVBracingFront(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimVBracingFrontSplice(CDomyun *pDom, CPlateBasicIndex *pBx, long nPos);

	void DimVBracingPlanTop(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY);
	void DimVBracingPlanLeft(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimVBracingPlanRight(CDomyun *pDom, CPlateBasicIndex *pBx);
	//수직브레이싱 상세도 
	void DrawVBracingDetail(CDomyun* pDom, CPlateBasicIndex* pBx, long nPos);
	void DrawVBracingDetailAA(CDomyun* pDom, CPlateBasicIndex* pBx, BOOL bDim);
	void DrawVBracingDetailBB(CDomyun* pDom, CPlateBasicIndex* pBx, BOOL bDim);

	void DimVBracingFrontAllGirder(CDomyun* pDom, CPlateBasicIndex* pBx);
	void DimVBracingDetail(CDomyun* pDom, CPlateBasicIndex* pBx, long nPos);
	void DimVBracingDetailOne(CDomyun* pDom, CPlateBasicIndex* pBx);
	void DimVBracingDetailTwo(CDomyun* pDom, CPlateBasicIndex* pBx);
	void DimVBracingDetailThree(CDomyun* pDom, CPlateBasicIndex* pBx);
	void DimVBracingDetailFour(CDomyun* pDom, CPlateBasicIndex* pBx);
	void DimVBracingDetailFive(CDomyun* pDom, CPlateBasicIndex* pBx);
	//가로보 전체거더의 한단면 생성
	void DrawCRBeamTotalFrontOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDrawGirder=FALSE, BOOL bJewon=FALSE, BOOL bGirderNo=FALSE);
	//거더상세도 도면용 단면
	void DrawTrimCRBeamOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	//입력화면생성
	void DrawInputCRBeamPlan(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bPaintMap=FALSE);
	void DrawInputCRBeamFront(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bPaintMap=FALSE);
	void DrawInputCRBeamSangse(CDomyun *pDom, CPlateBasicIndex *pBx);
	//가로보 부재
	void DrawCRBeamWebAndFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCRBeamSpliceOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeftDraw=TRUE, BOOL bRightDraw=TRUE);
	void DrawCRBeamVStiffnerOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawFrontHStiffBx(CDomyun *pDomP, CPlateBasicIndex *pBx);
	void DrawCRBeamGassetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	//가세트 부부재..
	void DrawCRBeamFrontGassetPlate_FlangeBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft);
	void DrawCRBeamFrontGassetPlate_WebLeftBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bBolt=FALSE, BOOL bBoltLine = FALSE, BOOL bFillCircle = FALSE);
	void DrawCRBeamFrontGassetPlate_WebRightBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bBolt=FALSE, BOOL bBoltLine = FALSE, BOOL bFillCircle = FALSE);
	void DrawCRBeamFrontGassetSupportPlateLeftBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCRBeamFrontGassetSupportPlateRightBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCRBeamFrontGassetSupportPlateByBoltLeftBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCRBeamFrontGassetSupportPlateByBoltRightBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCRBeamFrontGassetSupportPlateSubBx(CDomyun *pDom, CPlateBasicIndex *pBx, CDPoint BP, CDPoint EP, BOOL bLeft);
	//치수선(입력부분)
	void DimCRBeamSpliceTotal(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bShop, long nDimDan=1);
	void DimCRBeamSpliceMoment(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, long nDimDan=1, BOOL bTop=TRUE);
	void DimCRBeamSpliceShear(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, long nDimDan=1, BOOL bTop=FALSE);
	void DimCRBeamSpliceRight(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bShop, long nDimDan=1);
	void DimCRBeamSpliceLeft(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bShop, long nDimDan=1);
	void DimCRBeamPlanFlSp(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, double BX, double BY);
	void DimCRBeamPlanLeftRight(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY);
	void DimCRBeamPlanPaint(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY);	
	void DimCRBeamPlanTop(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY);
	void DimCRBeamFrontTop(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY, double angDeg=90);
	void DimCRBeamFrontBottom(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY, double angDeg=90);	
	void DimCRBeamFrontLeft(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY, double angDeg=90);
	void DimCRBeamFrontPaint(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY);
	void DimCRBeamFrontRight(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, double angDeg=90);
	void DimCRBeamGassetKneeBracing(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimCRBeamSangseOne(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimCRBeamSangseTwo(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimCRBeamFrontAllGirder(CDomyun *pDom, CPlateBasicIndex *pBx);
	//거더및 슬래브 라인
	void DrawGirderFront(CDomyun *pDom, CPlateBasicIndex *pBx, long nDrawingType=2);
	void DrawSlabLineFront(CDomyun *pDom, CPlateBasicIndex *pBx);
	//수직보강재(가로보)
	void DrawFrontVStiffnerBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawFrontVStiffnerBxPrv(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCRBeamAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx);
	//수직보강재(거더)
	void DrawGirderVStiffFrontOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	//수평보강재(전면)
	void DrawInputHBracingFront(CDomyun *pDom, CPlateGirderApp *pGir, long nPos);

	void BoltHole(CDomyun *pDom, CDPoint vAxis, CDPoint xyCen, double dRad, double dRatio=1);
  
};

#endif // !defined(AFX_APLATEDRAWCROSS_H__ACB96FA4_1701_44B6_BFF8_B198FBBD1036__INCLUDED_)
