// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawHBracing.h: interface for the CAPlateDrawHBracing class.
//  
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWHBRACING_H__6499E094_AC8B_4383_89ED_735487659217__INCLUDED_)
#define AFX_APLATEDRAWHBRACING_H__6499E094_AC8B_4383_89ED_735487659217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage; 
class CPlateBasicIndex;
class CPlateGirderApp;
class CHBracingGusset;
class AFX_EXT_CLASS CAPlateDrawHBracing 
{
public:
	CAPlateDrawHBracing(CDataManage *pDataMng);
	virtual ~CAPlateDrawHBracing();
private:
	CDataManage *m_pDataMng;
	//CBracing에서 6지점위치
	enum nGasPos {CUR_STT, CUR_MID, CUR_END, NEXT_STT, NEXT_MID, NEXT_END};
	// 1/4이음판의 중심에서 골조선방향
	enum nCenID14 {CEN_LU, CEN_RU, CEN_LD, CEN_RD};
	// 1/2이음판의 중심에서 골조선방향
	enum nCenID12 {CEN_LU_LD, CEN_LU_RU, CEN_RU_RD, CEN_LD_RD};

public:
	void DrawScallopWithThreePoint(CDomyun *pDom, CDPoint xyStt, CDPoint xyMid, CDPoint xyEnd, double dT, double dW);
	void DrawInputHBracingBySangseType(CDomyun *pDom,  CPlateGirderApp *pGir, long nPos, BOOL bSangseDom=FALSE, BOOL bJewon=FALSE, BOOL bInput =TRUE, BOOL bCut = FALSE);
	void DrawInputHBracingScallop(CDomyun *pDomD, double dThickSplice);
	void DimInputCompressionRib(CDomyun *pDom, double dThickSplice);
	void DimInputTensionRib(CDomyun *pDom, double dThickSplice);
	//SUB DRAWING FUNCTION
	void DrawBeamOnBracingPos(CDomyun *pDom, CPlateBasicIndex* pBx, BOOL bJewon = FALSE);
	void DrawBeamTypeV(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon = FALSE);
	void DrawBeamTypeVInv(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon = FALSE);
	void DrawBeamTypeX(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon = FALSE);
	void DrawBeamTypeSLOPLEFT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon = FALSE);
	void DrawBeamTypeSLOPRIGHT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon = FALSE);
	
	void DrawHGuessetBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawHGussetOnHGussetBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bOptionApply, long nOption,
		                       BOOL bBoltHole=TRUE, BOOL bBoltLine=TRUE, BOOL bSangSe = FALSE, BOOL bDim=FALSE, BOOL bJewon=FALSE, long nType=0, BOOL bInput=TRUE);
	void DimHGussetBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bBoltHole=TRUE, BOOL bInput =TRUE);
	void DrawHGussetPlateJewon(CDomyun *pDom, CHBracingGusset* pBrGu, CDPoint xyCen, long nID);
	
	//현재 횡거더가셋트의 윤곽을 표시한다.
	void DrawOutLineOfCurHGusset(CDomyun *pDom, CPlateGirderApp *pGir, long nPos, double dRad);
	void SetHBracingSpJewonTotal(CDomyun *pDom);

	void DrawCompScallop(CDomyun *pDomP, double BX, double BY, double ribT, double ribW, BOOL bDrawVStiff, double Length=0, BOOL bLower=TRUE, double slope=0.0, double sinVal=1.0);
	void DrawTensionScallop(CDomyun *pDomP, double BX, double BY, double ribT, double ribW, double Length=0, BOOL bLower=TRUE, double slope=0.0, double sinVal=1.0, BOOL bDrawVStiff=TRUE);

	BOOL GetXyMatchSegAndSegNoChangeRes(CDPoint xyStt1, CDPoint xyEnd1, CDPoint xyStt2, CDPoint xyEnd2, CDPoint& xyResult);
};

#endif // !defined(AFX_APLATEDRAWHBRACING_H__6499E094_AC8B_4383_89ED_735487659217__INCLUDED_)
