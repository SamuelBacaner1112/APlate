// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawDanmyun.h: interface for the CAPlateDrawDanmyun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWDANMYUN_H__F5B8AD2D_6BD9_4144_8C5A_B218A1595082__INCLUDED_)
#define AFX_APLATEDRAWDANMYUN_H__F5B8AD2D_6BD9_4144_8C5A_B218A1595082__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CDataManage;
class CJSolePlate;
class CPlateBasicIndex;
 
class AFX_EXT_CLASS CAPlateDrawDanmyun
{
public:
	CAPlateDrawDanmyun(CDataManage *pDataMng);
	virtual ~CAPlateDrawDanmyun();
private:
	CDataManage *m_pDataMng;
public:		
	///////////////
	void DrawGirderAllOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawGirderOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bPaintMap = FALSE, long nMode = 0);
	void DrawGirderBindConc(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bRebar=FALSE);
	void DimVStiffDet(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bStiff);
	///////////////

	void DrawInputGuardFence(CDomyun *pDom, long nIndex, BOOL bIsInput=TRUE);
	void DrawInputSolePlate(CDomyun *pDom, long nRow, long nGirder, long nJijum);
	void DrawInputScallop(CDomyun *pDom);
	void DrawInputHStiff(CDomyun *pDom, CPlateBasicIndex *pBxSp, CPlateBasicIndex *pBxSc, double scale, bool bDrawDetailDlg = TRUE);
	void DrawInputDanTaper(CDomyun *pDom, double dScale);
	void DrawInputStagingRing(CDomyun *pDom, int m_nType, BOOL bDim=TRUE);
	void DrawInputBracket(CDomyun *pDom, double dRound, double dChip, double R, double C);
	void DrawInputBracketStiff(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawInputBStiffWithBolt(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawInputLiftLug(CDomyun *pDom, long nRow, long nCol, bool bAddPlate, bool bSidePlate);
//	void DrawInputSlabDanmyun(CDomyun *pDom);
	void DrawInputSlabDimDanmyun(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawInputPlateWShift(CDomyun *pDom, double dScale);
	void DrawJackUpStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimJackUpStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawVStiffAllOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawManufactureVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft);
	void DrawSpliceOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput=TRUE);
	void DrawLaneMark(CDomyun *pDom, CPlateBasicIndex *pBx);

	void DrawSlabUpper(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bWalkRoad=TRUE, BOOL bPave=TRUE);
	void DrawSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawSlabHunchOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimSlabUpper(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput=TRUE);
	void DimSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimSlabUserHunch(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimSlabCenterLine(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDimText = TRUE);
	void DimSlabHunchWidth(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimSlabBaseThick(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawGuardWall(CDomyun *pDom, CPlateBasicIndex *pBx);
	
	void DrawPlanSolePlateBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bHalf, BOOL bSttHalf, BOOL bEndHalf);

	void DrawDanmyunOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bWalkRoad=TRUE, BOOL bPave=TRUE, long nDrawingType=2, BOOL bGirder=TRUE);
	void DrawDanmyunEttOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon, BOOL bGirderMark, BOOL bMarkPosUp);
	void DrawCRBeamFrontAllOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon, BOOL bGirderMark, BOOL bMarkPosUp);
	//
	void DrawTensionBar(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput=FALSE);	
	void DrawTensionBar_ExcelOut(CDomyun *pDom, CPlateBasicIndex *pBx, CStringArray& szArr, CDPointArray& xyArr, CDoubleArray& AngArr, BOOL bInput=FALSE);	
	void DrawTensionBarStartUnit(CDomyun *pDom, CDPoint ptStt, BOOL bLeft);
	void DrawTensionBarEndUnit(CDomyun *pDom, CDPoint ptStt, BOOL bLeft);
	void DimTensionBarOnLineInfo(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput=FALSE);
	void DimTensionBar(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput=FALSE);

	// Typical Section 치수선
	void DimTypicalSectionUpper(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimTypicalSectionBottom(CDomyun *pDomP, CPlateBasicIndex *pBx, long dAddLen=0, BOOL bApplyMirror=FALSE);
	void DimTypicalSectionSlabThick(CDomyun *pDomP,CPlateBasicIndex *pBx);

	void DimFrontHDanExtTop(CDomyun *pDomP, CPlateBasicIndex *pBx, long nSttDan=0);
	void DimFrontHDanExtBottom(CDomyun *pDomP, CPlateBasicIndex *pBx, long nSttDan=0);
	void DimTextSlopeArrow(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxStt=NULL,  CPlateBasicIndex *pBxEnd=NULL, BOOL bMirror=FALSE);	

public:	
	//방호벽 관련...
	void DstDrawGuardFence(CDomyun *pDomP, CPlateBasicIndex *pBx, long nHDan, BOOL bActual = FALSE);
	void DstDimGuardFenceTop(CDomyun *pDomP, CPlateBasicIndex *pBx, long nHDan, BOOL bInput);
	void DstDimGuardFenceSide(CDomyun *pDomP, CPlateBasicIndex *pBx, long nHDan, BOOL bInput);
	//횡단면제원
	void DrawHDanBaseLineForInput(CDomyun *pDom);	
	void DrawHDanSlabForInput(CDomyun *pDom);
	void DrawHDanGirderForInput(CDomyun *pDom, long nJijum, BOOL bRealHeight=TRUE);	
	void DimBindConcJewonForInput(CDomyun *pDom, long nJijum);	// 구속콘크리트 설정
	void DrawHDanDimForInput(CDomyun *pDom, long nJijum=0);
	void DrawHDanJewonDimForInput(CDomyun *pDom, long nGir, long nJijum, CString strJijum);
	void DimHeightGirder(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeftDim=TRUE, BOOL bReverse=FALSE, BOOL bBindDim=FALSE, double dMaxH=-1, double dMinH=-1);	// 거더 높이
	void DimHeightGirderAll(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeftDim=TRUE, BOOL bReverse=FALSE, BOOL bBindDim=FALSE);	// 대표 거더에서 높이표시
	//방호벽
	void DrawGuardWall(CDomyun *pDom, CCentSeparation *pGWall, double BX, double BY);
	void DimGuardWallSide(CDomyun *pDom, CCentSeparation *pGWall, double BX, double BY,  BOOL bLeft, BOOL bInput, double DimScale);
	void DimGuardWallTop(CDomyun *pDom, CCentSeparation *pGWall, double BX, double BY, BOOL bInput,  double DimScale);
	//솔플레이트
	void DrawSolePlateDetail(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bGen, long nGirder, long nJijum);
	void DrawSolePlatePlanDetail(CDomyun *pDom, long nGir, long nJijum);
	void DimSolePlatePlan(CDomyun *pDom, BOOL bGen, long nGir, long nJijum);
	void DrawSolePlateSideDetail(CDomyun *pDom, CPlateBasicIndex *pBx, long nGir, long nJijum);
	void DimSolePlateSide(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bGen, long nGir, long nJijum);
	void DrawShoeBoxDetail(CDomyun *pDom, double BX=0, double BY=0, double dFactorThick=1.0, BOOL bLongitude = TRUE, double dBogus = 0, long nGir=0, long nJijum=0);
	void DimShoeBoxDetail(CDomyun *pDom, double BX=0, double BY=0, BOOL bInput=TRUE, long nGir=0, long nJijum=0);
	//스캘럽
	void DrawScallopGen(CDomyun *pDom, double R);
	void DimScallopGen(CDomyun *pDom, double R);
	//수평보강재
	void DrawHStiff_withSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double scale, bool bDrawDetailDlg = TRUE);
	void DrawHStiff_withVStiff(CDomyun *pDom, CPlateBasicIndex *pBx, double scale, bool bDrawDetailDlg = TRUE);
	//공장이음세부
	void DrawFactDetail_VType(CDomyun *pDom,double T1,double T2, double Scale
									, long nUpDown,double termW, double termH
									,double AngleV, BOOL bCenter, BOOL bDimDraw, BOOL bShop = FALSE, BOOL bDim = TRUE);
	void DimFactDetail_VType(CDomyun *pDom,double T1,double T2, double Scale, long nUpDown
					  ,double termW, double termH,double AngleV, BOOL bCenter, BOOL bDimDraw, BOOL bShop = FALSE);

	//동바리고리
	void DrawFrontCarryRingType1Bx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, BOOL bDetail = TRUE);
	void DimFlangeType(CDomyun *pDom, BOOL bDim);
	void DrawFrontCarryRingType2Bx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, BOOL bDetail = TRUE);
	void DimIlbanType(CDomyun *pDom, BOOL bDim);
	//블라켓
	void DrawGassetPlatePlan(CDomyun *pDom, double BX, double BY, double W1, double W2, double dis1, double dis2, 
							double R, double C, BOOL bLeft, BOOL bFront, CDPoint vAng =CDPoint(0,1), CDPoint vRotate=CDPoint(1,0), 
							BOOL bRoundOnly=FALSE, double dWeld=0);
	//블라켓 보강재 세부
	void DrawBoxPart(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawBoxPartWithBolt(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawGassetPlate(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawGassetPlateWithBolt(CDomyun *pDom, CPlateBasicIndex *pBx);
	//들고리 도면
	void DrawLiftLugFrontView(CDomyun *pDom, long nRow, long nCol, bool bAddPlate, bool bSidePlate);
	void DrawLiftLugSidePlateView(CDomyun *pDom, long nRow, bool bAddPlate, bool bSidePlate);
	void DimFrontView(CDomyun *pDom, CPlatePieceObject *pObj);
	void DimSideViewSidePlate(CDomyun *pDom, CPlatePieceObject *pObjSide);
	void DimAddPlate(CDomyun *pDom,CPlatePieceObject *pObjAdd);
	void DrawSideMainPlateView(CDomyun *pDom, long nRow, bool bAddPlate, bool bSidePlate);
	//Jackup soleplate
	void DrawJSolePlate(CDomyun *pDom, long nGirder, long nJijum, long nPos);
	void DimJSolePlate(CDomyun *pDom, long nGirder, long nJijum, long nPos);
	void DrawJSolePlatePlane(CDomyun *pDom, double dBaseX, double dBaseY);
	void DrawJSolePlateSide(CDomyun *pDom, double dVSlop, double dBaseX, double dBaseY);
	void DimJSolePlatePlane(CDomyun *pDom, double dBaseX, double dBaseY);
	void DimJSolePlateSide(CDomyun *pDom, double dVSlop, double dBaseX, double dBaseY);
	//상하판너비변화부
	void DrawPlateWShift(CDomyun *pDom, double dW1, double dW2, double dScale, BOOL bUpper, BOOL bCenter, double dRatioH, double dRatioW);
	void DimPlateWShift(CDomyun *pDom, double dW1, double dW2, double dScale, BOOL bUpper, BOOL bCenter, BOOL bDimDraw, double dRatioH, double dRatioW);
	// 하부교좌면
	void DrawPierPlace(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bDrawGirderMark=FALSE);

	// 스터드
	void DrawFrontUpperStudOrAnchorBx(CDomyun *pDomP, CPlateBasicIndex *pBx, long nStudType, BOOL bInput = TRUE);
	void DrawDimFrontUpperStudOrAnchorBx(CDomyun *pDomP, CPlateBasicIndex *pBx, long nStudType, BOOL bInput = TRUE);

	void	DimGirderAndSlab(CDomyun *pDom, CPlateBasicIndex *pBx);
};	

#endif // !defined(AFX_APLATEDRAWDANMYUN_H__F5B8AD2D_6BD9_4144_8C5A_B218A1595082__INCLUDED_)
