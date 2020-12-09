// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutCrossBeam.h: interface for the CAPlateOutCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTCROSSBEAM_H__F0C6C26B_07F0_49EF_8118_751A4792DC2F__INCLUDED_)
#define AFX_APLATEOUTCROSSBEAM_H__F0C6C26B_07F0_49EF_8118_751A4792DC2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef CTypedPtrArray	<CObArray, CPlateCrossBeam*> CCrossList;

class CAPlateOutCrossBeam : public CDrawPage  
{
public:
	void DimFrontJewonTextHStiff(CDomyun *pDom, CPlateBasicIndex *pBx);
	CAPlateOutCrossBeam();
	virtual ~CAPlateOutCrossBeam();

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();
	void AddPageSummaryTable(CDomyun *pDom);
	void AddCrossBeamOneType(CDomyun* pDom, CPlateBasicIndex* pBx, long nType = 0);
	void AddCrossBeamTotalType(CDomyun* pDom, CPlateBasicIndex* pBx,  long nType = 0);
	
	void AddCrossBeamFrontAtTotalDanmyun(CDomyun* pDom, CPlateBasicIndex* pBx);
	void AddCrossBeamPyungAtTopTotalDanmyun(CDomyun* pDom, CPlateBasicIndex* pBx);
	void AddCrossBeamPyungAtBottomTotalDanmyun(CDomyun* pDom, CPlateBasicIndex* pBx);
	void AddCrossBeamSangseOne(CDomyun* pDom, CPlateBasicIndex* pBx);
	void AddCrossBeamSangseTwo(CDomyun* pDom, CPlateBasicIndex* pBx);
	void AddCrossBeamSangseOneGusset(CDomyun* pDom, CPlateBasicIndex* pBx);
	void AddCrossBeamSangseTwoGusset(CDomyun* pDom, CPlateBasicIndex* pBx);
	
	void MakeCrossBeamList();
	void MakeCrossBeamListAtFirstGirder();

	//단면별 전체 가로보
	CTypedPtrArray	<CObArray, CPlateCrossBeam*> m_CrossList;
	CTypedPtrArray	<CObArray, CPlateCrossBeam*> m_CrossListAtFirstGirder;
	void DrawFrontCrossMain(CDomyun* pDom, CPlateBasicIndex* pBx);
	void DrawPlanCrossMain(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawDetailCrossGasset(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper);
	void DrawDetailCrossVStiff(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawDetailAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawDetailBKSupport(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft);
	void DrawDetailCrossBKScallop(CDomyun *pDom, CPlateBasicIndex *pBx);
	//
	void DimDetailCrossGasset(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper);
	void DimDetailCrossVStiff(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimGassetSupPlateDetail(CDomyun *pDom, CPlateBasicIndex *pBx, double Length, double Thick, BOOL bLeft);
	void DimDetailCrossBKScallop(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, double ScallopR, double ScallopC,
											  double Length, double Slop1,  double Slop2,  double Thick1, double Thick2, 
											  double WebT, BOOL bLeft, BOOL bExtend, long nScallop);
	void DimTotalFrontJewonText(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimTotalPlanUpperJewonText(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimTotalPlanLowerJewonText(CDomyun *pDom, CPlateBasicIndex *pBx);
	
	//가로보 한개씩
	void DimFrontCrossMainTop(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimFrontCrossMainLeft(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimFrontCrossMainRight(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimTextFrontSlope(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimFrontCrossJewonText(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimWeldFrontCrossMain(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimFrontCrossGassetSupportLeft(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimFrontCrossGassetSupportRight(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimFrontBracketBolt(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCrossBeamFlange(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft);
	//
	void DimPlanCrossJewonText(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimPlanCrossMainTop(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimPlanCrossMainSplice(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimPlanCrossSpliceSide(CDomyun *pDom, CPlateBasicIndex *pBx);
	
	void AddDetailBMOneType(CDomyun *pDom, CPlateBasicIndex *pBx);
	void AddDetailBMTotType(CDomyun *pDom, CPlateBasicIndex *pBx);
	void AddKeyPan(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxCr, BOOL bSummary = FALSE);
	void DrawSummaryTable(CDomyun *pDom);
	void DrawMarkSummaryPlan(CDomyun *pDomP);
};

#endif // !defined(AFX_APLATEOUTCROSSBEAM_H__F0C6C26B_07F0_49EF_8118_751A4792DC2F__INCLUDED_)
