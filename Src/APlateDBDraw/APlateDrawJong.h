// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawJong.h: interface for the CAPlateDrawJong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWJONG_H__95097678_A50E_4B53_AAD9_8DAF76CE5B57__INCLUDED_)
#define AFX_APLATEDRAWJONG_H__95097678_A50E_4B53_AAD9_8DAF76CE5B57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const long DRT_TEXTJEWON    = 0x01;
const long DRT_DETMARK		= 0x02;

class CPlateBasicIndex;
class CDataManage;

class AFX_EXT_CLASS CAPlateDrawJong
{
public:
	CAPlateDrawJong(CDataManage *pDataMng);
	virtual ~CAPlateDrawJong();
	CDPoint		m_vRotate;
	CDoubleArray m_dArrStud;

private:
	CDataManage *m_pDataMng;
	// 추가(종단입력부분)
	double		m_SideSttY;
	long		m_Gir;
	long		m_nGsu;
	long		m_SW;
	CDPoint		m_CenterXY;
	double		m_rax,m_ray;	// 지간구성 및 지점각도입력의 비율 변수
	double		m_SideY;
	CDPoint		m_CurDimPoint;

	void InsertionSort(CDWordArray &Data);
protected:
	void DrawFacePlateStt(CDomyun *pDom);
	void DrawFacePlateEnd(CDomyun *pDom);

	void DrawTaperLine(CDomyun *pDom, CDPoint sp, CDPoint ep, BOOL bUpper, BOOL bSttTaper=FALSE, BOOL bEndTaper=FALSE);
public:
	void DrawAlternateSection(CDomyun *pDom, long nG, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bPlan=FALSE);
	
	void DrawJongGirderTotal(CDomyun *pDom);
	void DrawJongGirderTotalHStiff(CDomyun *pDom);
	void DrawJDanGirderForInput(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimJongStud(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
 	void DrawGirderLineBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine=2, double ExtendLine=0, double dFlangeThickScalce=1, double dGirderHeightScale=1, BOOL bBindHide=FALSE);
	void DrawGirderLineForInput(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double ExtendLine=0);	

	void DrawBindLineBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bBindHide);
	void DrawGirderLineUpperBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bThickLine, double ExtendLine=0, double dFlangeThickScalce=1, double dGirderHeightScale=1);
	void DrawGirderLineLowerBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bThickLine, double ExtendLine=0, double dFlangeThickScalce=1, double dGirderHeightScale=1);
	void DrawPaveLine(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDir);
	void DrawInOutGateBx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bFacePlate=FALSE, long nType = 0, double dFlangeThickScalce=1, double dGirderHeightScale=1);
	void DrawFactJongBx(CDomyun *pDomP,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bText = TRUE, BOOL bMark = FALSE, BOOL bSymType = TRUE);

	void DrawCrossBeamBrkFlangeBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawCrossBeamBrkFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt);
	void DrawHStiffBx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine=2, long nJewonType=-1, BOOL bBoth = FALSE);
	void DrawHStiffBxJewon(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLRDim=FALSE);
	void DrawHStiffOneBxJewon(CDomyun* pDomP, double StdSta, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxPrev,  CPlateBasicIndex *pBxNext, long nCol, BOOL *bWidthSmall, BOOL bLRDim);
	void DrawHStiffOneBx(CDomyun* pDomP, double StdSta, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, long nCol, BOOL *bWidthSmall, long nLine=2, long nJewonType = -1, BOOL bBoth = FALSE);	
	void DrawVStiffBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine=2, long nJewonType = -1, BOOL bApplyHiddenLine=FALSE);
	void DrawVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, long nCol, long nLine=2, long nJewonType = -1);
	void DrawVStiffBxJewon(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nCol,  BOOL bJewonLeftRigh =FALSE);
	void DrawJackUpBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine=2, long nJewonType = -1, BOOL bApplyHiddenLine=FALSE);
	void DrawJackUpOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, long nCol, long nLine=2, long nJewonType = -1);
	void DrawJiJumVStiffBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine=2, long nJewonType = -1, BOOL bApplyHiddenLine=FALSE);
	void DrawJiJumVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, long nCol, long nLine=2, long nJewonType = -1);
	void DrawHGuessetBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawHGuessetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt);

	void DrawInputJongSttEndPart(CDomyun *pDom, long nCurPos);
	void DimJongSttEnd(CDomyun *pDom, BOOL bStt);
	void DrawJongSttEnd(CDomyun *pDom, BOOL bStt);
	//현장이음복부판 상세
	void DrawInputDetSpliceWeb(CDomyun *pDom, long nG, long nSp, BOOL bTitle=TRUE, BOOL bPaintMap=FALSE);
	void DrawWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double BX=0, double BY=0, long nShape=0, BOOL bBoltLine=TRUE);
	void DrawWebSpliceShear(CDomyun *pDom, CPlateBasicIndex *pBx,  double BX=0, double BY=0, long nShape=0, BOOL bBoltLine=TRUE);
	void DrawWebSpliceMoment(CDomyun *pDom, CPlateBasicIndex *pBx, double BX=0, double BY=0, long nShape=0, BOOL bBoltLine=TRUE, BOOL bUpper=TRUE);
	void DrawFlangeLineWeb(CDomyun *pDom, CPlateBasicIndex *pBx, double BX=0, double BY=0, BOOL bFlange=TRUE);	
	void DimWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bTitle=TRUE, BOOL bExcel=FALSE, BOOL bPaintMap=FALSE);
	
	void DrawFieldSpliceBx(CDomyun* pDomP,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bGeneral=FALSE, BOOL bDetail = FALSE, BOOL bSttDraw = TRUE, BOOL bMark=TRUE, BOOL bCenterLineOnly=FALSE, double ySeperateAlways = 12, BOOL bBoltHole=FALSE, BOOL bBoltLine=FALSE, BOOL bDrawFlangeSplice=FALSE, BOOL bChangeLayer=TRUE);
	void DrawFieldSpliceHatchJong(CDomyun* pDomP,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd , long Dir, BOOL bCenterLineOnly, BOOL bSpliceMark, double dFlangeThickScalce=1, double dGirderHeightScale=1, BOOL bRealDraw=TRUE, long nType = -1);
	
	//교대 교각
	void DrawPier(CDomyun *pDomP, long nPier,  long nG, CDPoint MoveXY, BOOL bCut=FALSE, BOOL bMark=TRUE, BOOL bSimpleType=FALSE);
	void DrawPierSide(CDomyun *pDomP, long nPier, long nG, BOOL bCut, BOOL bMark, BOOL bSimpleType);
	void DrawAbutSide(CDomyun *pDomP, long nPier, long nG, BOOL bCut, BOOL bMark);
	// 슈
	void DrawShoeMarkBx(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double Scale, BOOL bGeneral=FALSE, double ySeperateRatio = 1, BOOL bHatch = FALSE);

	// 용접
	void AddWeldJong(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,double rotAng, BOOL bLeft, BOOL bWeldMarkOne = TRUE);
	void AddWeldJongFlange(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,double rotAng, long nSide, CHAR &cWeldType, BOOL bGrindUpper=TRUE, BOOL bGrindLower=TRUE, BOOL bDetail=TRUE);
	void AddWeldSplicePos(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng, CHAR &cWeldType, BOOL bGrindUpper=TRUE, BOOL bGrindLower=TRUE, BOOL bDetail=TRUE);
	void AddWeldJongCrossBeam(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng);

	// 스터드
	void DrawStudBx(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide);

	void AbutPierSymbol(CDomyun *pDom, long nG);
	void CrossBeamSide(CDomyun *pDom, long nG);
	void TextCrossBeamHeight(CDomyun *pDom, long nG);
	void VStiffSide(CDomyun *pDom, long nG);
	void TextVStiffHeight(CDomyun *pDom, long nG);
	void GirderNumberingSide(CDomyun *pDom, long nG);
	void FlangeSide(CDomyun *pDom, long nG, BOOL bUpper);
	void SpliceSide(CDomyun *pDom, long nG);
	void LeftFactSide(CDomyun *pDom, long nG);
	void SideCenter(CDomyun *pDom);
	BOOL DrawGirderNum(long nG);
	void DrawSideAll(CDomyun *pDom, long nG);
	void DrawByunSide(CDomyun *pDom, long nG);
	void DimSideLeftWidth(long nG, CDomyun *pDom);
	void DimSideRightWidth(long nG, CDomyun *pDom);
	double GetSideSttY(long nG, long nCount);
	void SetCenterAngle();
	double GetLengthSide(double sta, double width, long nG);
	double GetMaxHeight(long nG);
	double GetStationKyungMaxMin(long kyung, BOOL bMin=TRUE);
	double SttEndTHeight(long nG, long pos=1);	// 시.종점 박스높이(박스 변화부)
	double ByunDanCurveDraw(CDomyun *pDom, CPlateGirderApp *pGir, long nG, double sttlen, double endlen, double height, double width);
	void AddSttDimJongBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void AddEndDimJongBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);

	void SetCursorPosition(long nG, double staStt, double staEnd, CDomyun *pDom, BOOL bDelCursor=TRUE, double dLineThick=150, long nPos=0, long nLineColor=-1);
};

#endif // !defined(AFX_APLATEDRAWJONG_H__95097678_A50E_4B53_AAD9_8DAF76CE5B57__INCLUDED_)
