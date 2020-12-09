// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateGirderApp.h: interface for the CPlateGirderApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEGIRDERAPP_H__FB87F4F9_527E_4B70_AD5C_97B9C15E906A__INCLUDED_)
#define AFX_PLATEGIRDERAPP_H__FB87F4F9_527E_4B70_AD5C_97B9C15E906A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBasicIndex;
//typedef CTypedPtrArray <CObArray, CPlateBasicIndex *> CPlateBasicIndexArray;

#include "PlateBasicIndex.h"

class AFX_EXT_CLASS CPlateGirderApp : public CPlateGirder  
{
public:	
	CPlateGirderApp();
	CPlateGirderApp(CPlateBridge* pDB,CLineInfo* pLine,CPlateGirderApp* pLeft = NULL);
	virtual ~CPlateGirderApp();
	


public:	
	void SetLKPOS(long nLK);

public:
	CPlateGirderApp& operator=(const CPlateGirderApp& obj);

	CPlateBasicIndex* GetBxCrossBeamOrVBracing(BOOL bCrossBeamOrVBracing, long nCount) const;
	CPlateBasicIndex* GetBxCrossBeamHasHunch() const;
	long GetQtyCrossBeamOrVBracing(BOOL bCrossBeamOrVBracing) const;
	BOOL IsCrossBeamBxHasHunch(CPlateBasicIndex *pBx);

public:
	long GetNumberGirder() const;
	CPlateFactChain	*GetFactChain(long nMarkBujeFact, long nCount) const;
	CCamber			*GetCamberByNode(long nNode) const	{ return m_pCamber[nNode]; }

	void MakeBracing(long nCount);
	void MakeVStiffSection(long nCount);
	void MakeCrossSection(long nCount);
	void MakeJiJumStiff(long nCount);

public:	
	void MakeSplice(long nMake);
	void MakeFactChain(long nMake,long nType);
	void MakeCamber(long nMake);
	void MakeJackupStiff(long nMake);
	void MakeWebStiff(long nMake);
	void MakeFactChainUpp(long nMake);
	void MakeFactChainLow(long nMake);
	void MakeFactChainWeb(long nMake);	

	double GetHeightGirder(CPlateBasicIndex *pBx, BOOL bReal) const;
	double GetDisPlanPlateWebToWeb(CPlateBasicIndex* pBx);
	double GetDisFrontPlateWebToWeb(CPlateBasicIndex* pBx);
	double GetWidthOnStation(double dSta, BOOL bUpper, BOOL bNext = TRUE) const;
	double GetThickOnStation(double dSta, BOOL bUpper, BOOL bNext = TRUE) const;
	double GetLengthSharpOnBx(CPlateBasicIndex* pBx, BOOL bUpper);
	double GetDisGirderToGirder(CPlateBasicIndex* pBx);
	double GetGiulPlateCrossGusset(CPlateBasicIndex* pBx);
	double GetGiulCrossBeamCenter(CPlateBasicIndex* pBx);
	double GetGiulWebSplice(CPlateBasicIndex *pBx, BOOL bUpper);

	long GetQtyVStiffSectionTotal() const { return m_VStiffSectionArr.GetSize(); }
	long GetQtyCrossSectionTotal() const { return m_CrossSectionArr.GetSize(); }
	long GetQtyCrossBeamVBracing() const;
	long GetQtyCrossBeamVBracingHunch() const;
	long GetQtyCrossBeamOnly() const;
	long GetQtyVBracingOnly() const;
	long GetQtyCrossAType() const;
	long GetQtyCrossCType() const;
	long GetQtyCrossBType() const;
	long GetQtyCrossPType() const;
	long GetQtyBracingReal() const;
	long GetQtyBracingAll() const {return m_BracingArr.GetSize();}
	long GetQtyHGusset(BOOL bReal=TRUE) const;
	long GetQtyVStiff() const;
	long GetQtySplice(long nSpType=CPlateGirder::TOTAL) const;
	long GetQtyNodeCamber() const			{ return m_pCamber.GetSize();}
	long GetQtyFactChain(long nMarkBujeFact) const;


	CPlateBasicIndex* GetBxBySectionType(long nSectType, long nPos) const;
	CPlateBasicIndex* GetBxCrossBeamOnly(long nCount) const;
	CPlateBasicIndex* GetBxVBracingOnly(long nCount) const;

	//Shoe
	CShoe *GetShoe(long nJiJum)		{ return &m_Shoe[nJiJum]; }
	
	CVStiff *GetJijumVStiffByJijum(long nJijum, long nVStiff) const;
	CBracing *GetBracingInTotal(long nPos) const;
	CBracing *GetBracingByHBrGussetPos(long nPos);
	long GetBracingPosByHBrGussetPos(long nPos);

	CHBracingGusset *GetHBracingGussetInTotal(long nPos) const { return nPos < m_HGussetArr.GetSize() ? m_HGussetArr.GetAt(nPos) : NULL; }
	CPlateSplice    *GetSpliceInTotal(long nPos) const { return nPos < m_pSplice.GetSize() ? m_pSplice.GetAt(nPos) : NULL; }

	CPlateGirderApp *GetNextGirder() const;	

	void SetBxPosMatchCross();
	CPlateBasicIndex	*GetBxMatchByCrossBeam(CPlateBasicIndex *pBx, CPlateGirderApp *pGir) const;
	POS GetPOSMatchByCross(POS psBx, CPlateGirderApp *pGir) const;

	CPlateBasicIndex	*GetBxMatchByVStiff(CPlateBasicIndex *pBx, CPlateGirderApp *pGir) const;
	POS GetPOSMatchByVStiff(POS psBx, CPlateGirderApp *pGir) const;

	// 제원함수 추가
	double GetWidthPlateSolePlateJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double GetLengthPlateSolePlateJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double GetThickPlateSolePlateJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double GetJewonAndProperty(long nMarkBuje, long nID);

	// VBracing Gusset
	double GetWidthVBracingGussetJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double GetLengthVBracingGussetJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double GetThickVBracingGussetJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	// HBracing Gusset
	double GetWidthHBracingGussetJewon(CPlateBasicIndex *pBx, long nRow, long nCol=0, DIMTYPE nDimType=DIM_BASIC) const;
	double GetLengthHBracingGussetJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	double GetThickHBracingGussetJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	// CrossBeam Bracket Gusset Stiff
	double GetLengthCrossBeamWeb(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double GetLengthCrossBeamFlange(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	
	double GetLengthCrossBeamBracketWeb(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double GetLengthCrossBeamBracketPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	double GetLengthCrossBeamBracketGasset(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double GetLengthCrossBeamBracketGassetPlate(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double GetLengthCrossBeamVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	double GetLengthCrossBeamHStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront) const;

	double GetWidthCrossBeamWebPlate(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double GetWidthCrossBeamFlangeUpper(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double GetWidthCrossBeamFlangeLower(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double GetWidthCrossBeamBracketGassetPlate(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double GetWidthCrossBeamBracketWebJewon(CPlateBasicIndex* pBx, long nCol, DIMTYPE nDimType) const;
	double GetWidthCrossBeamBracketPlate(CPlateBasicIndex* pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	double GetWidthCrossBeamBracketGasset(CPlateBasicIndex* pBx, long nCol, DIMTYPE nDimType) const;//가로보 브라켓 보강재

	double GetWidthCrossBeamWebSpliceMoment(CPlateBasicIndex* pBx, DIMTYPE nDimType) const;
	double GetWidthCrossBeamWebSpliceShare(CPlateBasicIndex* pBx, DIMTYPE nDimType) const;
	double GetWidthCrossBeamWebSpliceFiller(CPlateBasicIndex* pBx) const;

	CPlateBasicIndex *GetBxVBracingByNo(long nPos);
	CPlateBasicIndex *GetBxATypeByNo(long nPos);
	CPlateBasicIndex *GetBxPTypeByNo(long nPos);
	CPlateBasicIndex *GetBxBTypeByNo(long nPos);
	CPlateBasicIndex *GetBxCTypeByNo(long nPos);


	double GetThickVBracingSteelJewon(CPlateBasicIndex*pBx, long nMarkBuje) const;
	CString GetStringVBracingBeamJewon(CPlateBasicIndex *pBx, long nCol) const;
	CString GetStringHBracingBeamJewon(CPlateBasicIndex *pBx, long nCol, BOOL bUpper=FALSE) const;
	CString GetStringCrossHSteelJewon(CPlateBasicIndex *pBx) const;

	CDPoint FindAngleSectionLeft(CPlateBasicIndex *pBx) const;
	CDPoint FindAngleSectionRight(CPlateBasicIndex *pBx) const;
	double	FindStaCrossLeft(CPlateBasicIndex *pBx) const;
	double	FindStaCrossRigh(CPlateBasicIndex *pBx) const;


// 재정의
public:	
	virtual CPlateBasicIndex* newBx(CPlateBasicIndex *pBx = NULL);	
/*	void MakeFactChainUpp(long nMake);
	void MakeFactChainLow(long nMake);
	void MakeFactChainWeb(long nMake);
*/	void MakeHGusset(long nMake);
		
	CShoe *GetShoeByJijumNo(long nJijumNo);		

	double GetThickVStiff(CPlateBasicIndex *pBx, BOOL bLeft) const;
	double GetLengthVStiff(CPlateBasicIndex *pBx, BOOL bLeft, DIMTYPE nDimType) const;
	double GetWidthVStiff(CPlateBasicIndex *pBx, BOOL bLeft) const;	
	
// Overiding
public:	
	double	GetGiulJongDanShoe(CPlateBasicIndex *pBx) const;
	double GetGiulCrossBeam(CPlateBasicIndex* pBx);

	void GetBxWithenStation(CTypedPtrArray <CObArray, CPlateBasicIndex *> &ptrArray, double staStt, double staEnd, __int64 bxFindFlag/* = 0*/) const;
	//void GetBxWithenStation(CPlateBasicIndexArray *ptrArray, double staStt, double staEnd, __int64 bxFindFlag/* = 0*/) const;

public:
	////////////////////////////////////////////////////////////////
	CPlateBasicIndex *GetBxMaxFlangeWidth();
	CPlateBasicIndex *GetBxMinFlangeWidth();
	
	double GetMaxPlanEL(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd) const;
	double GetMinPlanEL(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd) const;

	double GetAngleSlabAnchor(double dAnchorT, double duM) const;	

	// 단면 TAPER 관련
	long	GetQtySectionTapered(CPlateBasicIndex *pBx) const;
	BOOL	IsSectionTapered(CPlateBasicIndex *pBx) const;
	long	GetCrossBxSu(CPlateBasicIndex *pBx, double D, __int64 BxFlag) const;
	//
	double	GetSizeWeldFillet(CPlateBasicIndex *pBx, long nMarkBuje, long nMarkBujeSec, BOOL bSttSplice = FALSE) const;
	CDPoint GetXyGirderDis(double sta, double SD=0, CDPoint vAng = CDPoint(0,1)) const;
//********start 제원(너비, 두께, 길이)구할 경우에 아래 함수만 사용할 것.
	CVector GetJewonTotal(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_BASIC, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);
	double	GetLengthJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_BASIC, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE) const;
	double	GetWidthJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_BASIC, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE) const;
	double	GetThickJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_BASIC, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE) const;

	double	GetLengthDimType(double sta1, double sta2, double dis=0, BOOL bUpper=TRUE, DIMTYPE nDimType=DIM_BASIC) const;	
	double	GetLengthCrossTaper(CPlateBasicIndex *pBx, DIMTYPE nDimType=DIM_BASIC) const;
	double	GetHeightCrossTaper(CPlateBasicIndex *pBx) const;	
	double	GetHeightTaper(double sta, double D=0, CDPoint vAng = CDPoint(0,1)) const;


	double	GetDisStringer(CPlateBasicIndex *pBx, long nStr = 0) const;	
	double	GetDisCRSttToCREndReal(CPlateBasicIndex *pBx, DIMTYPE nDimType=DIM_BASIC) const;
	double	GetDisToCenterLineAct(double sta, double D=0, CDPoint vAng= CDPoint(0,1)) const;
	double	GetDisToCenterLineForExcel(double sta, double D=0, CDPoint vAng= CDPoint(0,1)) const;
	double	GetDisLevelAct(double sta, double D=0, CDPoint vAng= CDPoint(0,1)) const;

	double	GetWidthSlabLeftDom(CPlateBasicIndex *pBx = NULL) const;
	double	GetWidthSlabRightDom(CPlateBasicIndex *pBx = NULL) const;
 	
	void	GetVertexFlangeByStation(CVectorArray &vArr, double staStt, double staEnd, BOOL bUpper, long nPos, __int64 BxType) const;

	void	GetVertexFlangeByModel(CVectorArray &vFlange, double staStt, double staEnd, BOOL bUpper, __int64 BxType = 0, BOOL b3DModel = TRUE) const;
	void	GetVertexWebByModel(CVectorArray &vWeb, double staStt, double staEnd, __int64 BxType = 0, BOOL b3DModel = TRUE) const;
	void	GetVertexFlange(CVectorArray &vFlange, CPlateBasicIndex *pBx, BOOL bUpper) const;
	void	GetVertexWeb(CVectorArray &vWeb, CPlateBasicIndex *pBx) const;	
	void	GetVertexPointByDis(CVectorArray &vFlange, double staStt, double staEnd, double widL, double widR, BOOL bUpper) const;

	// 가로보 제원관련
	long	GetPointVStiffOnCrossBeam(CPlateBasicIndex *pBx, CDPoint Po[]/*20*/, BOOL bShopCut=FALSE, BOOL bIncludeSide=TRUE) const;
	long	GetPointVStiffOnCrossBeamPrv(CPlateBasicIndex *pBx, CDPoint Po[]/*20*/, BOOL bShopCut=FALSE, BOOL bIncludeSide=TRUE) const;		
	
	double	GetStationByOneStepMid(double dSta, long nDir, __int64 bxFindFlag = BX_CROSSBEAM_VBRACING | BX_VSTIFF) const;
	void	GetStationArray(CDPointArray &StaArr, double dSttSta, double dEndSta, __int64 BxFlag) const;
	double	GetStationStt() const;//Bx 이전(수직보강재 위치입력)에만 사용할 것.
	double	GetStationEnd() const;//Bx 이전(수직보강재 위치입력)에만 사용할 것.
	double	GetStationOnJijum(long nJijum) const;
	double	GetStationBySc(double sta,double SD, CDPoint vAng) const;
	double	GetStationByActLength(double sta, double len, double dis=0) const;
	double	GetStationCenterLine(double sta, double D=0, CDPoint vAng= CDPoint(0,1)) const;
	double	GetStationCenLineForEl(double sta, double D=0, CDPoint vAng= CDPoint(0,1)) const;
	double	GetStationCenLineOrGirLine(double sta, double D=0, CDPoint vAng= CDPoint(0,1)) const;
	double	GetPyungubaeCenLineOrGirLine(double sta, double D=0, CDPoint vAng= CDPoint(0,1)) const;

	double	GetStationFactEndByFactNo(long nMarkBujeFact, long nFact) const;
	double	GetStationOffset(double dSta, CDPoint vAng, double dLengthLine) const;
	double	GetStationMatchByCRLine(double staCR, CDPoint vAngCR) const;
	void	GetStationArrayByFlangeSection(CDPointArray &StaArr, double dSttSta, double dEndSta, double D, __int64 BxFlag, __int64 BxCross = 0) const;

	double	GetSSS(SLABGIRDERSHOE nTarget, double dDist=0) const;//GetStationFromSttEnd()로 함수명 수정
	double	GetSssDis(BOOL bStt, BOOL bOffset, double dLenMove, long nHunchType) const;
	double	GetSssDisOrdinaryLine(double dStaCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const;
	double	GetSssDisExtLineSymple(double dStaCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const;
	double	GetSssDisExtLine(double dStaCR, CDPoint vAngCR, BOOL bOffset, double dLenMove, double dLenW) const;

	CDPoint GetAAA(SLABGIRDERSHOE nTarget, double dDist=0) const;//GetAngleFromSttEnd()로 함수명 수정
	CDPoint	GetAaaDis(BOOL bStt, BOOL bOffset, double dLenMove, long nHunchType) const;
	CDPoint	GetAaaDisOrdinaryLine(double dStaCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const;
	CDPoint	GetAaaDisExtLineSymple(double dStaCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const;
	CDPoint	GetAaaDisExtLine(double dStaCR, CDPoint vAngCR, BOOL bOffset, double dLenMove, double dLenW) const;

	CDPoint GetAngleStt() const;//Bx 이전(수직보강재 위치입력)에만 사용할 것.
	CDPoint GetAngleEnd() const;//Bx 이전(수직보강재 위치입력)에만 사용할 것.
	CDPoint GetAngleOnJijum(long nJijum) const;
	CDPoint GetAngleOffset(double dSta, CDPoint vAng, double dLengthLine) const;
	CDPoint	GetAngleMatchByCRLine(double staCR, CDPoint vAngCR) const;
	CDPoint GetAngleGirderCenterToCenterLine(double sta) const;

	// 각 위치별 Elevation
	double	GetElevation(CPlateBasicIndex *pBx, double dis) const;
	double	GetElevationGirderForReal(CPlateBasicIndex *pBx, long nRow, long nCol);
	double	GetElevationPlanElOnCenLineAct(double sta, double D=0, CDPoint vAng = CDPoint(0,1)) const;
	double	GetElevationPaveFlangeLeftOrRight(double sta, double D=0, CDPoint vAng = CDPoint(0,1)) const;	
	double	GetElevationPaveAct(double sta, double D=0, CDPoint vAng = CDPoint(0,1)) const;
	double	GetElevationFlange(double sta, double D=0, CDPoint vAng = CDPoint(0,1), BOOL bUpper=TRUE) const;	
	double	GetElevationFlangeCamber(double sta, double D=0, CDPoint vAng = CDPoint(0,1), BOOL bUpper=TRUE) const;	
	double	GetElevationDom(CPlateBasicIndex *pBx) const;
	double	GetElevationDomForExtLine(CPlateBasicIndex *pBx) const;
	double	GetElevationPaveWeb(double sta, double D=0, CDPoint vAng = CDPoint(0,1)) const;
	double	GetElevationWeb(double sta, double D=0, CDPoint vAng = CDPoint(0,1), BOOL bUpper=TRUE) const;

	double	GetThickSlabPave(double sta, double D=0, CDPoint vAng = CDPoint(0,1));
	double	GetThickPyungubae(double sta, double D=0, CDPoint vAng = CDPoint(0,1)) const;
	
	double	GetHeightGirderByStaAng(double sta, double D=0, CDPoint vAng = CDPoint(0,1), BOOL bReal = TRUE, BOOL bSlab=FALSE) const;
	double	GetHeightFlangeUpperByTaper(double sta, double D=0, CDPoint vAng = CDPoint(0,1), BOOL bSlab=FALSE) const;
	double	GetHeightFlangeLower(double sta, double D=0, BOOL bReal = FALSE) const;
	
	double	GetHeightMaxByJijum() const;
	double	GetHeightInOutUpper(BOOL bStt, BOOL bReal = TRUE) const;
	double	GetDisCrossbeamByFolding(CPlateBasicIndex *pBx, BOOL bJong = TRUE) const;

	CPlateBasicIndex	*GetBxMatchByDiap(CPlateBasicIndex *pBx, CPlateGirderApp *pGir) const;
	CPlateBasicIndex	*GetBxMatchByVert(CPlateBasicIndex *pBx, CPlateGirderApp *pGir, __int64 bxFlag=NULL) const;
	CPlateBasicIndex	*GetBxByStation(double sta,__int64 bxFindFlag = 0) const;
	CPlateBasicIndex	*GetBxByStationDir(double sta,long nDir,__int64 bxFindFlag = 0) const;

	CPlateBasicIndex	*GetBxOnJijum(long nPier) const;
	CPlateBasicIndex	*GetBxBySpliceNo(long nSp) const;
	CPlateBasicIndex	*GetBxBySpliceNoSPType(long nSp) const;
	CPlateBasicIndex	*GetBxBySpliceNoWeldType(long nSp) const;

	CPlateBasicIndex	*GetBxBySpliceNoCenter(long nSp) const;
	CPlateBasicIndex	*GetBxByCrossNo(long nCross) const;
	CJackupStiff		*GetJackupByJijum(long nJijum, long nJackup) const;
	CPlateBasicIndex	*GetJackupBxByJijumBx(CPlateBasicIndex *pBx, long nJackup, long nSide) const;
	long GetQtyJackupByBx(CPlateBasicIndex *pBx, long nDir) const;
	long GetQtyJijumVStiffByBx(CPlateBasicIndex *pBx);

	long	GetSpliceNoByBx(CPlateBasicIndex *pBx) const;

	
	double	GetPyungubaeCenLine(double sta, double D, CDPoint vAng = CDPoint(0,1)) const;
	double	GetPyungubaeFromLeft(double sta, double D, CDPoint vAng = CDPoint(0,1)) const;
	double	GetGiulJongDanOnlyLine(double sta, double D=0, CDPoint vAng = CDPoint(0,1)) const;
	double	GetGiulCamber(double sta, double D, CDPoint vAng) const;
	double	GetGiulFlangeUpperLower(double sta, double D = 0, BOOL bUpper = TRUE) const;
	double	GetGiulFlangeUpperLowerCamber(double sta, double D = 0, BOOL bUpper = TRUE) const;
	double	GetGiulWebCamber(double sta, double D = 0, BOOL bUpper = TRUE) const;
	double	GetGiulJongDanShoe(CPlateBasicIndex *pBx, BOOL bFirShoe) const;
	double	GetGiulCrossBody(CPlateBasicIndex *pBx, BOOL bShop) const;
	double	GetGiulCrossGasset(CPlateBasicIndex *pBx, BOOL bShop, BOOL bLeft) const;	

//	long	GetNumberCrossBeamByBxBefore(CPlateBasicIndex *pBx) const;	
	CString GetNumberSpliceForOut(CPlateBasicIndex *pBx, BOOL bNext=FALSE) const;
	long	GetNumberSpliceByBx(CPlateBasicIndex *pBx) const;
	long	GetNumberJijumByBx(CPlateBasicIndex *pBx) const;
	long	GetNumberJijumByStation(double sta) const;
	long	GetNumberJiganByStation(double sta) const;
	long	GetFactNumberByStationBefore(double sta, long nMarkBujeFact) const;

	double	GetCamberByStation(double sta, long nType) const;
	long	GetQtyCamberByUser() const;
	long	GetCamberNodeByStation(double dSta) const;
	double	GetMaxCamberInGirder() const;
	double	GetFactorByByunDan(double sta, double D=0) const;	
		
//	CStringer	*GetStringerByBxForLastCrossbeam(CPlateBasicIndex *pBx, long nRow=0) const;
	BOOL	IsStringerUseByBx(CPlateBasicIndex *pBx) const;
	BOOL	IsStringerUseByCrossNo(long nCross) const;// 세로보의 유무
	BOOL	IsByunDanMyunInJigan(long nJiganNo) const;
	BOOL	IsBxObjectInMidByStation(double sta, __int64 BxFlag);
	BOOL	IsSameTypeBySplice(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec, BOOL bSimple) const;
	BOOL	IsByunByTerm(double StaStt, double StaEnd) const;
	BOOL	IsByunByStation(double sta) const;
	long	GetStudBoltSu(long nSp);
	BOOL	IsExtLineGirder() const;
	long	GetSizeFactChain(long nMarkBujeFact);

// Constructor
public:	
	CVector GetVectorNormalOnFlange(double sta, double D, BOOL bUpper = TRUE) const;
	double GetThickFactByStation(double sta, long nMarkBujeFact, BOOL bNext = FALSE) const;
	double GetHeightDiffBySectionDist(CPlateBasicIndex *pBx, double DistFromLeftEnd, BOOL bUpper=TRUE, BOOL bShop=TRUE) const;
	CString GetRadiusString(double sta, double dis) const;	

//	double	FindStaCrossLeft(CPlateBasicIndex *pBx) const;
//	CDPoint FindAngleCrossLeft(CPlateBasicIndex *pBx) const;
//	double	FindStaCrossRight(CPlateBasicIndex *pBx) const;
//	CDPoint FindAngleCrossRight(CPlateBasicIndex *pBx) const;	
	POS GetPOSNext(POS pos,long nCount=1) const;
	POS GetPOSMatchByDiap(POS psBx, CPlateGirderApp *pGir) const;
	POS GetPOSMatchByVert(POS psBx, CPlateGirderApp *pGir, __int64 bxFlag=NULL) const;
	double GetStationByPOS(POS psBx) const;	
	POS GetPOSByStation(double sta,__int64 bxFindFlag = 0) const;
	POS GetPOSByStationDir(double sta,long nDir,__int64 bxFindFlag = 0) const;
	POS GetPOSHead();
	POS GetPOSTail();
	void PressBxList(const CPtrList& InputList, __int64 bxFlag = 0,CDWordArray* pLKException=NULL);			

// Implementation
protected:
	//Girder, Bx에서만 사용
	CPlateBasicIndex *GetBxByStationDistDir(double sta,long nDir,__int64 bxFindFlag = 0, double dist = 0) const;	
	//
	void	ContextOutBasicIndex(CString sPathName = _T("")) const;
	double	GetLengthFlangeLowerByByun(double x1, double x2, long nJiganNo, double dLengthByunOnly, double dHeightByunMax) const;
	double	GetHeightByunType(long nJiganNo, double len, long nByunType) const;
	double	GetStationOnPier(long nPier) const;
	CDPoint GetAngleOnPier(long nPier) const;

	double	GetDisLevelLeftAct(double sta, double D, CDPoint vAng) const;
	double	GetDisLevelRightAct(double sta, double D, CDPoint vAng) const;

	void InsertBx(const CPtrList& RList);
	CPlateBasicIndex* InsertBx(const CPlateBasicIndex *pBx);
	void	PressBxListForOneFlag(const CPtrList& InputList,__int64 bxFlagOnlyOne,CDWordArray* pLKException);
	void	GetUsedFlagArrInPressBxList(CPtrArray& UsedFlagArr,__int64 bxFlag) const;

	// Bx HashTable	
	//void	InsertTableByBxFlag(CPlateBasicIndex *pBx);
	//void	ModifyMapList(__int64 BxFlag);

//***************  길이(치수관련 함수)
	double	GetLengthByDisGir(double sta1, double sta2, double dis=0) const;
	double	GetLengthFlangeLowerBySta(double staStt, double staEnd, double D) const;
	double	GetLengthFlangeByStaJong(double staStt, double staEnd, double D, BOOL bUpper, BOOL bJong) const;
	double	GetLengthFlangeByStaJongBase(double staStt, double staEnd, double D, BOOL bUpper, BOOL bJong) const;

	//***************  길이 제원(치수관련 함수)
	double	GetLengthFlangeUpperJewon(CPlateBasicIndex *pBx, BOOL bUpper, DIMTYPE nDimType) const;
	double	GetLengthFlangeWebJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;	
	double	GetLengthHStiffJewon(CPlateBasicIndex *pBx, long nRow,long nCol, BOOL bLeft, DIMTYPE nDimType) const;										
	double	GetLengthCrossWebJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double	GetLengthCrossFlangeJewon(long nMarkBuje, CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double	GetLengthCrossBracketWebJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double	GetLengthCrossBracketPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	double	GetLengthCrossBracketGasset(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double	GetLengthCrossBracketGassetPlate(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double	GetLengthFacePlateJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const;	
	
	double	GetWidthFlangeUpperJewon(CPlateBasicIndex *pBx, BOOL bUpper, DIMTYPE nDimType) const;
	double	GetWidthFlangeWebJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;	
	double	GetWidthHStiffJewon(CPlateBasicIndex *pBx, BOOL bLeft, DIMTYPE nDimType) const;				
	double	GetWidthSpliceWebMomentJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	double	GetWidthSpliceWebShareJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;			
	double	GetWidthCrossFlangeUpper(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double	GetWidthCrossFlangeLower(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double	GetWidthCrossWebPlate(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double	GetWidthCrossBracketWebJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double	GetWidthCrossBracketPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const;
	double	GetWidthCrossBracketGasset(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const;
	double	GetWidthCrossWebSpliceMoment(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double	GetWidthCrossWebSpliceShare(CPlateBasicIndex *pBx, DIMTYPE nDimType) const;
	double	GetWidthSolePlateJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const;
	
	double	GetThickFactChainJewon(long nMarkBujeFact, CPlateBasicIndex *pBx) const;
	double	GetThickSolePlateJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const;
	double	GetThickSolePlateJackupJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const;

	CDPoint GetRectangleFlangeByBx(CPlateBasicIndex *pBx, BOOL bUpper) const;
	CDPoint GetRectangleFlangeWebByBx(CPlateBasicIndex *pBx) const;
	CDPoint GetRectangleJRib(CPlateBasicIndex *pBx, long nRow, BOOL bUpper, BOOL bLiner=FALSE) const;

	double	GetGiulByunDan(CPlateBasicIndex *pBx) const;

public:
	double GetValueBasicDefault(CString szName);
	long GetCrossBeamsu();
	long GetVBracingsu();
	CPlateBasicIndex* GetCrossBxAtMaxFlangeWidth();
	CPlateBasicIndex* GetCrossBxAtMinFlangeWidth();
	double GetMaxWidthFlange(BOOL bUpper) const;
	double GetMinWidthFlange(BOOL bUpper) const;
	CVStiff* GetVStiffByPos(long nPos);
	CPlateBasicIndex* GetBxAllSafeHDan();
	long GetQtyRowJijumVStiffByBx(CPlateBasicIndex *pBx);
	long GetQtyRowJackupByBx(CPlateBasicIndex *pBx);
	long GetFactConnectType(CPlateBasicIndex *pBx, long nMarkBuje) const;
	////////////////////////////////////////////////////////////
	// 일반제형관련 추가함수
	double GetWidthFlangeUpper(CPlateBasicIndex *pBx) const;
	double GetWidthFlangeLower(CPlateBasicIndex *pBx) const;
	////////////////////////////////////////////////////////////
	void GetHunchXyDom(CPlateBasicIndex *pBx, CDPoint* A, BOOL bApplyFlangeOpt=TRUE, BOOL bJijumHunchApply=FALSE, BOOL bExceptTUBlock=FALSE) const;		
	double	GetDisCenterLineToGirderDom(CPlateBasicIndex *pBx) const;	
	double	GetHunchXyHeightDom(CPlateBasicIndex *pBx, BOOL bLeft) const;
	double	GetHunchXyHeightNodeDom(CPlateBasicIndex *pBx, long nNode) const;

	CPlateBasicIndex* GetBxByVBracingPos(long nPos);
	long GetQtyVBracing();
	long GetVBracingPosByBx(CPlateBasicIndex *pBx);

	double GetHeightGirderByJijum(long nJ, BOOL bReal=TRUE) const;
	double GetHeightGirderBySta(double dSta, BOOL bReal=TRUE) const;
	
	double GetThickFlangeByBase(double dSta, BOOL bUpper=TRUE) const;
	double GetThickFlangeByBase(long nJ, BOOL bUpper=TRUE) const;
	
	void SetHeightGirderAtJijum(long nJ, double dH);
	double GetThickSlab(CPlateBasicIndex *pBx, double dDist);
	CDPoint GetXyPlanSlabCrossbeamBase(CPlateBasicIndex *pBx, BOOL bLeft) const;				//복부판 내측과 만나는 가로보 중심선의 좌우 좌표를 구함
	CSection* GetSectionCRandVBByNo(long nSec) const;
	CSection* GetSectionBySectionNo(long nSec) const;
	BOOL	  IsExistTaperArea(CPlateBasicIndex *pBx, double dDist) const;
	long      GetNumberJijumMinGirderEL() const ;

	//////////////////////////////////////////////////////////////////////////
	// TU연속교 추가작업
	long	GetQtyJoint()	const;
	// TRUE : 현장이음위치 에서 설정한 교번부-교번부 사이의 부모멘트부
	BOOL	IsShiftSection(CPlateBasicIndex *pBx, BOOL bNext=TRUE);
	BOOL	IsShiftSection(double dSta, BOOL bNext=TRUE);
	// return : pBx가 속한 정모멘트부의 구간길이, dLenCalc : 정모멘트 구간내에서 모멘트 계산길이
	double	GetLengthPositiveSection(double dSta, double &dLenCalc);

};

#endif // !defined(AFX_PLATEGIRDERAPP_H__FB87F4F9_527E_4B70_AD5C_97B9C15E906A__INCLUDED_)
