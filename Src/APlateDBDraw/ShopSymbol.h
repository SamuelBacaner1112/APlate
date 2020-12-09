// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ShopSymbol.h: interface for the CSymbolDom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOL_H__819129C1_8FD4_11D4_8F75_0020741155A2__INCLUDED_)
#define AFX_SYMBOL_H__819129C1_8FD4_11D4_8F75_0020741155A2__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARoadOptionStd;
class CAPlateDrawStandard;
class CPlateBasicIndex;

class AFX_EXT_CLASS CSymbolDom : public CDomyun  
{
public:
	CSymbolDom();
	CSymbolDom(CDomyun *pDom, CARoadOptionStd* pStd, CAPlateDrawStandard* pDBStd=NULL);
	virtual ~CSymbolDom();
	
	CARoadOptionStd *m_pOptStd;
	CAPlateDrawStandard *m_pDBStd;

public:	
	// 도면과 기능은 같고 심볼로 처리 //////
	void SymbolTextCircle(CDPoint xy, CString str, BOOL bSameSize);
	void SymbolTextCircle(CDPoint xy, CString str, CString sBlockName, CDPoint vAngRot, BOOL bSameSize=FALSE);
	void SymbolTextCircleWithLine(CDPoint xy, CString str, CDPoint LineStt, double Radius=-1, double TextHeight=-1);
	void SymbolGiSiArrowExtend(CDPoint xy, double Dis1, double Dis2, long nDirArrow, CString str1, CString str2, CString sBlockName, CDPoint vAngRot);
	////////////////////////////////////////

	void DrawWeldMark(CDPoint xy,long nMarkBuje1, long nMarkBuje2, CPlateBasicIndex *pBx, double dWeldSize, double dHeight1, double dHeight2, BOOL bLeft, BOOL bUpper, double dAngle, BOOL bFrontFace = TRUE, CString szAddStr = "", BOOL bGrind=TRUE, BOOL bSttSplice=FALSE);
	void SymbolTextCircle(CDPoint xy, CString str, double Radius=-1, double TextHeight=-1);
	void DrawTitleOnTable(double xp, double yp, CString sTitle, long nLayerType = HCAD_SUB_TITLE, BOOL bTopPos=TRUE, long nQtyLine=1);

	void DrawSymbolStationFlag(CDPoint xy, CString strUpper, CString strLower, double Height, double Length=-1, CDPoint rotAngle=CDPoint(1,0));
	void DrawSymbolBoltBox(double xpos, double ypos, CString str1, CString str2);
	void DrawSymbolGiSiLine(double X , double Y, double Dis, BOOL bLeft, BOOL bUpper, double Height1, double Height2, CString cs);		
	void DrawSymbolShoeMarkPlanGen(double x , double y, long nType, double Ang);
	void DrawSymbolShoeMarkPlan(long nRow, long nCol, BOOL bBlock=FALSE,  BOOL bMoveAndRotate=TRUE, BOOL bScaleApply=TRUE);
	void DrawSymbolHexagonStr(CDPoint xy, CDPoint rAng, CString str, BOOL bBlock=FALSE);
	void DrawSymbolDiamondMark(CDPoint xy, CDPoint rAng, CString Str, BOOL bBlock=FALSE);

	void DrawSymbolThickMark(double xpos, double ypos, CString cs, BOOL bBlock = FALSE);
	void DrawSymbolDanMyunMark(double xpos, double ypos, CString cs, long Dir, BOOL bBlock = FALSE);
	void DrawSymbolCLMark(double xpos, double ypos,BOOL VerDirection = TRUE, BOOL bBlock = FALSE);
	void DrawSymbolVentPlan(CDPoint xy, CDPoint rvAng, BOOL bText=FALSE, BOOL bUpText=TRUE);
	void DrawSymbolDetailMark(double xpos, double ypos, double Height, CString UpStr, CString DnStr, CString csText, BOOL bUpper=TRUE, BOOL bLeft=FALSE, double Ang = 0);
	void DrawSymbolSectionGisiArrow(CDPoint xy, double dDis, CString strTitle1, CString strTitle2, double dRotAng = 0, double dScale=100, BOOL bLeft=TRUE, BOOL bRight=TRUE);
	void DrawSymbolTitle(double xpos, double ypos, CString Title, CString UpStr, CString DnStr = _T("-"), CString DnTitle = _T(""));
	void DrawSymbolTitleExt(double xpos, double ypos, CString Title, CStringArray &strArr);
	void DrawSymbolTitleExt(double xpos, double ypos, long nSu, CString Title, CString sAssem, CString UpStr, CString DnStr= _T("-"));
	void DrawSymbolSPMark(double xpos, double ypos, double Height, CString Str, double AngDegRotate, BOOL bBlock=FALSE, BOOL bUpper=TRUE, BOOL bSmall=TRUE);
	void DrawSymbolJijumMark(double xpos, double ypos, CString Str, double AngDegRotate, BOOL bBlock = FALSE);
	void DrawSymbolShopMark(double xpos, double ypos, CString Str, double AngDegRotate, double dWidth = 0, BOOL bBlock = FALSE, BOOL bEllipse=TRUE);
	void DrawSymbolGirderMark(CDomyun *pCheckDom, double xpos, double ypos, CString Str, double AngDegRotate, BOOL bBlock = FALSE, long nInit = 0);
	void DrawSymbolNote1(double xpos, double ypos);
	void DrawSymbolNote2(double xpos, double ypos);
	void DrawSymbolCuttingS(double xpos, double ypos, double Angle);
	void DrawSymbolCircleMark(double xpos, double ypos, double Height, CString UpStr, CString DnStr, CString csText, BOOL bUpper=TRUE, BOOL bLeft=FALSE, double Ang = 0);
	void DrawSymbolCuttingMark(CDPoint Point, double dRadius, double Angle = 0);
	void DrawSymbolPropName(double xpos, double ypos, CString sText, double Angle);
	
	void DrawSymbolSectionMark(double BX, double BY, CString csText, BOOL bLeftText/*TRUE*/, double AngleDeg, double TextAngleDeg);
	void DrawSymbolDirectionArrow(CDPoint cp, CString cs, BOOL bFront);
	void DrawSymbolLinePointMark(CDPoint bp, CDPoint ep, CString szSta, CString szTypeText, CString szLeftType, CString szRightType);
};

#endif // !defined(AFX_SHOPSYMBOL_H__819129C1_8FD4_11D4_8F75_0020741155A2__INCLUDED_)
