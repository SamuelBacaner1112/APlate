// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateBridgeApp.h: interface for the CPlateBridgeApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEBRIDGEAPP_H__AA715BEF_2E84_440F_9D8B_306CD1349872__INCLUDED_)
#define AFX_PLATEBRIDGEAPP_H__AA715BEF_2E84_440F_9D8B_306CD1349872__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateGirderApp;
class CSlabApp;
class CLineInfo;
class CDataManage;
class CPlateBasicIndex;
class CShoe;

// 지점에서의 일정 떨어진 거리의 각부분별 명칭
enum SLABGIRDERSHOE
{
	OFF_STT_STT, OFF_STT_SLAB, OFF_STT_EXP, OFF_STT_COVER, OFF_STT_GIRDER, OFF_STT_SHOE, OFF_STT_HUNCHSTT, OFF_STT_HUNCHEND,
	OFF_END_END, OFF_END_SLAB, OFF_END_EXP, OFF_END_COVER, OFF_END_GIRDER, OFF_END_SHOE, OFF_END_HUNCHSTT, OFF_END_HUNCHEND
};

//슬래브는 여기서 생성한다.
class AFX_EXT_CLASS CPlateBridgeApp : public CPlateBridge  
{
public:	
	CPlateBridgeApp(CLineInfo* pLine = NULL);
	virtual ~CPlateBridgeApp();

public:
	typedef enum { SLAB_LEFT = 0, SLAB_RIGHT = 1 } SLAB_TYPE;
	enum { HDANTYPE_LEVER = 0, HDANTYPE_WALL = 1, HDANTYPE_VALRENGTH = 2 };
	enum { GUSSET_T, GUSSET_S1, GUSSET_S2, GUSSET_S3, GUSSET_W, GUSSET_DA, GUSSET_DB,
		GUSSET_DC, GUSSET_UA, GUSSET_UB, GUSSET_UC, GUSSET_B, GUSSET_H, GUSSET_R, 
		GUSSET_ST, GUSSET_DD, GUSSET_DE, GUSSET_UD, GUSSET_UE, GUSSET_GEN};
// Member
protected:
	CTypedPtrArray <CObArray, CSlabApp*>	m_SlabArr;	// 슬래브 배열	- 슬래브는 항상 2개(거더처럼 취급)

public:	
	CDataManage *m_pDataManage;	

	void Serialize(CArchive &ar);

public:
	// 기본함수 Make()
	double GetStationBridgeStt()	const { return (m_dStationBridgeStt); }
	double GetStationBridgeEnd()	const { return (m_dStationBridgeStt + m_dLengthTotalSlab + m_dLengthUganStt + m_dLengthUganEnd); }	
	double GetThickPave()			const { return m_dThickPave; }
	double GetSlabThick(long nSide) const { return (nSide == SLAB_LEFT) ? m_dThickSlabLeft : m_dThickSlabRight; }
	CDPoint GetAngleBridgeCenter()	const;
	long GetAbutSu() const;
	long GetPierSu() const;
	BOOL IsAbutOnJiJum(long nJijum) const;
	CDPoint GetXyPlateShoe(long nG, long nJijum);
	// Slab 관련
	void MakeSlab();
	CSlabApp *GetSlab(SLAB_TYPE nSlab) const { return m_SlabArr.GetAt(nSlab); }
	CSlabApp *GetSlabLeft() const { return GetSlab(SLAB_LEFT); }
	CSlabApp *GetSlabRight() const { return GetSlab(SLAB_RIGHT); }		
	virtual long GetQtyHDanReal(BOOL bActual, double staCR, CPlateBasicIndex *pBx = NULL) const;
	long GetQtyHDanAct(double staCR) const;
	long GetQtyHDanDom(CPlateBasicIndex *pBx) const;

	double GetWidthSlabLeft() const;
	double GetWidthSlabRight() const;
	
	// 새로 지원된 함수
	BOOL	IsSafeCross(long nRow, long nCol, BOOL bCheckPier = TRUE, BOOL bCheckCrossRight = TRUE, BOOL bCheckCrossLeft = TRUE) const;
	BOOL	IsSafeCrossByPier(long nRow, long nCol) const;	
	//가셋트보강판 제원
	double GetGussetPlateJewon(CPlateBasicIndex* pBx, int nWeld, long nJewon);

	void	GetXySss(SLABGIRDERSHOE nTarget, CDPoint* A) const;
	CString	GetStationForStringOutDB(double Sta, long nDecimalPoint = 3, BOOL bMarkSTA=TRUE) const;

	long	GetIndexBaseWeld(long nMarkBujeFir, long nMarkBujeSec, CPlateBasicIndex *pBx);
	long	GetIndexBaseScallop(long nMarkBuje1st, long nMarkBuje2nd, long nMarkBuje3rd, CPlateBasicIndex *pBx);
	CString GetMaterialSteelSymbol(long nMarkBuje) const;
	CString GetMaterialByThick(long nMarkBuje, double dThick) const;
	CString GetMaterial(long nMarkBuje, CPlateBasicIndex *pBx, long nRow = 0, long nCol = 0) const;
	CString GetMaterialNumber(long nMarkBuje, long nPos = 0) const;
	double	GetWeightTotal(long nMarkBuje, CPlateBasicIndex *pBx, CVector vJ, long nQty = 1) const;
	double	GetWeightUnit(long nMarkBuje, CPlateBasicIndex *pBx, CVector vJ) const;
	double	GetWeightUnitKgM(long nMarkBuje, CPlateBasicIndex *pBx, CVector vJ) const;

	CString GetMarkBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, BOOL bFront=TRUE) const;	
	CString GetMarkWeld(long nMarkWeld, CPlateBasicIndex *pBx) const;
	CString GetMarkWeldDetail(long nMarkWeld, CPlateBasicIndex *pBx) const;

	double	GetWidthAct(double sta) const;
	void	GetXyPaveAct(double sta, CDPoint A[HDANLINESU]) const;
	void	GetXyPaveActWithoutJongPlanEL(double sta, CDPoint A[HDANLINESU]) const;	
	double	GetDisSlabLeftAct(double sta) const;
	double  GetDisSlabRighAct(double sta) const;
	double	GetDisSlabLeftLevelAct(double sta) const;
	double	GetDisCenLineToCenSlabAct(double sta) const;
	double	GetElevationPaveActByDis(double staCR, double D) const;
	double	GetElevationPaveDomByDis(CPlateBasicIndex *pBx, double D) const;
	double	GetElevationCantilever(double staCR, BOOL bLeft) const;
	double	GetElevationPaveActByXY(CDPoint xy) const;
	double	GetPaveMaxEleAct(double sta) const;
	double	GetPaveMinEleAct(double sta) const;
	double	GetWidthDiap90Cross90(CPlateBasicIndex *pBx) const;
	double	GetWidthDiapAngCrossAng(CPlateBasicIndex *pBx) const;
	double	GetLengthHDanAct(double sta, long nHDanNode) const;
	long	GetTypeLevelHDan(long nHDanNode) const;
	long	GetWallGuardTypeHDan(long nHDanNode) const; 
	long	GetTypeVariableLengthHDan(long nHDanNode) const;
	BOOL	IsGuard(long nHDanNode, long nTypeHDan);	// 방호벽, 중분대가 있는지 검토
	long	GetValueTypeHDan(long nHDanNode, long nTypeHDan) const; 
	void	SetValueTypeHDan(long nHDanNode, long nTypeHDan, long nValueHDan) ;
	long	GetQtyHDanNodeByCenterLinePositionAct(CPlateBasicIndex *pBxGir = NULL) const;
	long	GetQtyHDanNode() const;
	long	GetQtyGirder(){ return m_pGirder.GetSize();}
	long	GetQtyJijumStiff(long nJijum);
	void	SetQtyJijumStiff(long nJijum, long nQtyJijumStiff);
	void	SetQtyHDanNode(long nQtyHDanNode);
	double	GetThickFrPaveByXy(CPlateBasicIndex *pBxGir, CDPoint xy) const;

	double	GetLengthBridge() const {return m_dLengthUganStt+m_dLengthTotalSlab + m_dLengthUganEnd;}
	double  GetScallopRadius(double T1, double T2) const;
	double	GetSlopeApplyBase(BOOL bJong) const;    //종경사 도면 적용기준
	long	GetBridgeType() const;
	BOOL	IsPlateGir();
	BOOL	IsSosuGir();
	BOOL	IsTUGir();
	BOOL	IsTUContinuousSpan();
	//방호벽 관련..
	void    GetXyGuardFenceDom(CPlateBasicIndex *pBx, long nHDan, CDPoint A[9], BOOL bActual = FALSE) const;
	BOOL	IsTypeGuardFenceDom(long nHDan) const;
	long	GetNumHDanByGuardFence(CCentSeparation *pGuard) const;
	long	GetNumGuardFenceFrLeft(long nHDan) const;
	CDPoint	GetXyHDanLeftDom(CPlateBasicIndex *pBx, long nHDan) const;
	long	GetNumSlabNodeDom(CPlateBasicIndex *pBxGir, long nHDan) const;

// Constructor
public:
	void SetLineInfo();

	BOOL	IsBolt(long nMarkBuje);
	BOOL	IsPlate(long nMarkBuje);
// Implementation
protected:
	BOOL	IsMaterialBolt(long nMarkBuje) const;		
	BOOL	IsMaterialStudWeld(long nMarkBuje) const;			
	BOOL	IsMaterialReBar(long nMarkBuje) const;
	BOOL	IsMaterialElbow(long nMarkBuje) const;	
	BOOL	IsMaterialSleeveForPipe(long nMarkBuje) const;
	BOOL	IsMaterialBeam(long nMarkBuje) const;
	BOOL	IsMaterialGirder(long nMarkBuje) const;

	double	GetWeightUnitBolt(long nMarkBuje, CVector vJ) const;
	double	GetWeightUnitStudWeld(long nMarkBuje, CVector vJ) const;
	double	GetWeightUnitStudBolt(long nMarkBuje, CVector vJ) const;
	double	GetWeightUnitExpandedMetal(long nMarkBuje, CVector vJ) const;	
	double	GetWeightUnitReBar(long nMarkBuje, CVector vJ) const;
	double	GetWeightUnitElbow(long nMarkBuje, CVector vJ) const;	
	double	GetWeightUnitSleeveForPipe(long nMarkBuje, CVector vJ) const;
	double	GetWeightUnitHBeam(long nMarkBuje, CVector vJ) const;

	long	GetNumberFixShoeRow(long nRow, long nCol)const;
	long	GetNumberFixShoeCol(long nRow, long nCol)const;
	long	GetNumberFixShoeByHyunRow(long nRow, long nCol)const;
	
public:
	BOOL IsByunDanInBridge();
	void SetCalcJongTendon();
	double GetBoltRad(CString szBoltSize);
	virtual long	GetNumberStartGirder() const;
	virtual long	GetNumberStartSplice() const;
	virtual double	GetWeightUnitPlate(long nMarkBuje) const;
	
	BOOL	IsSafeDiap(long nRow, long nCol, BOOL bCheckPier = TRUE, BOOL bCheckDiapRight = TRUE, BOOL bCheckDiapLeft = TRUE) const;
	BOOL	IsSafeDiapByPier(long nRow, long nCol) const;
	BOOL	IsSafeDiapByCrossBeam(long nRow, long nCol, BOOL bRight) const;
	double	GetStationByLengthInGrid(BOOL bSttShoe, long nG, double dTotalLen, int nLengthType, double dDisCenter) const;
	double	GetLengthByStationInGrid(BOOL bSttShoe, long nG, double sta, int nLengthType, double dDisCenter) const;
	double	GetStationInDBGrid(long nGridType, long nRow, long nCol) const;

	CPlateBasicIndex *GetBxOnNearByPoint(CDPoint xyPoint, __int64 bxFlag = 0, BOOL isHunCh = TRUE);
////////////////////////////////////////////////////////////////////////
//	SHOE 관련함수
	CShoe*	GetShoeByRowCol(long nRow,long nCol)const;
	BOOL	IsFirstShoe(long nRow, long nCol) const;
	CDPoint GetAngleShoeOnGirCenter(long nRow, long nCol, BOOL bJijumStiff = FALSE)const;	
	CDPoint GetXyShoe(long nRow, long nCol) const;
	double  GetDisShoeOnGirderCenter(long nRow, long nCol) const;
	double  GetStationCenLineForElByShoe(long nRow, long nCol) const;
	double	GetStationShoeOnGirCenter(long nRow, long nCol)const;
	long	GetNumberGirderByShoe(long nRow, long nCol) const;	
	long	GetQtyShoeOnJijum(long nJijum) const;
	long	GetQtyShoePrevious(long nRow, long nCol) const;
	//
	double	GetPyungubeCenLineByShoe(long nRow, long nCol) const;
	double	GetDisToCenterLineByShoe(long nRow, long nCol) const;
	double  GetDisRoadCenToSlabCen(CPlateBasicIndex *pBx);
	double	GetDisLevelActOnShoe(long nRow, long nCol) const;
	double	GetElevationOnBridgeSeatTop(long nRow, long nCol) const;
	double  GetElevationOnShoeTop(long nRow, long nCol) const;
	double	GetElevationOnMortarTop(long nRow, long nCol) const;
	double	GetElevationPlanElOnCenLineByShoe(long nRow, long nCol) const;
	double	GetHeightGirderByShoe(long nRow,long nCol, BOOL bReal=FALSE) const;
	double	GetThickSlabAndPaveByShoe(long nRow,long nCol) const;
	double	GetThickPyungubae(long nRow, long nCol) const;

	double	GetThickFlangeByShoe(long nRow, long nCol, long nMarkBuje) const;

	// 가상함수
	void	GetXyPaveUpperDom(CPlateBasicIndex *pBx, CDPoint A[HDANLINESU]) const;		
	double	GetWidthSlabDom(CPlateBasicIndex *pBxGir) const;
	double	GetWidthDiap90CrossAng(CPlateBasicIndex *pBx) const;
	double	GetDisSlabLeftDom(CPlateBasicIndex *pBxGir) const;
	double	GetDisSlabRightDom(CPlateBasicIndex *pBxGir) const;
	double	GetDisSlabLeftLevelDom(CPlateBasicIndex *pBxGir) const;
	double	GetLengthHDanDom(CPlateBasicIndex *pBxGir, long nHDanNode) const;
	double	GetHeightSlabFromRoadCenterDom(CPlateBasicIndex *pBxGir, double D) const;
	double  GetThickSlabAndPaveByDis(CPlateBasicIndex *pBxGir, double D, BOOL bJijumHunchApply = FALSE) const;
	CDPoint	GetMatchPavePointDom(CPlateBasicIndex *pBxGir, CDPoint xy, double slope, BOOL bLowSlab = TRUE) const;
	CDPoint GetMatchRebarPoint(BOOL bActual, BOOL bLowBySlabHunch, CDPoint xy, double slope, double staCR, double dSpace, CPlateBasicIndex *pBx = NULL) const;
	CDPoint GetMatchRebarPointDom(CPlateBasicIndex *pBxGir, CDPoint xy, double slope, double dSpace, BOOL bLowBySlabHunch) const;

	virtual void GetXyPave(BOOL bActual, CDPoint A[HDANLINESU], double staCR, CPlateBasicIndex *pBx = NULL) const;
	virtual void GetXyPaveRail(BOOL bRail, BOOL bActual, CDPoint A[HDANLINESU], double staCR, CPlateBasicIndex *pBx = NULL) const;
	virtual void GetXyPaveDom(CPlateBasicIndex *pBx, CDPoint A[HDANLINESU]) const;

	// 재정의된 함수들
	long GetMarkBujeByMarkBujeAndBx(long nMarkBuje, CPlateBasicIndex *pBx) const;
	CString GetMaterial(long nMarkBuje) const;
	double GetStationShoeOnCRLine(long nRow, long nCol) const;
	double GetDisSlopActOnShoe(long nRow, long nCol) const;

	BOOL IsPyunGubaeSame(CPlateBasicIndex *pBxpre, CPlateBasicIndex *pBx);
	BOOL IsWidthSame(CPlateBasicIndex *pBxpre, CPlateBasicIndex *pBx);
	BOOL IsStraightLineTotalBridge() const;
	long GetNumInputNodedeDom(CPlateBasicIndex *pBxGir, long nHDanDom) const;

	// 방호벽 관련
	BOOL IsExistLeftGuardFence();
	BOOL IsExistRightGuardFence();
	long GetGuardFencesu();
	CCentSeparation *GetGuardFence(long nGuard);
	CCentSeparation *GetGuardFencLeft();
	CCentSeparation *GetGuardFencRight();
	long GetCountOfCenterGuardFence();
	BOOL IsExistCenterGuardFence();
	CCentSeparation *GetGuardFencCenter(long nIdx);

	//
	double	GetRadiusMinTotalBridge() const;
	void	GetXySlabAct(double sta, CDPoint A[HDANLINESU]) const;
	void	GetXyFrontCantilever(CPlateBasicIndex *pBx, CDPoint* A, BOOL bLeft);
	long	GetsuCrossBeamBxHasHunch();
	BOOL	IsUserSlabHunch(BOOL bLeft, CPlateBasicIndex *pBx);

	void	SetInsultionVStiff();

	//슬래브(횡) 강선 형태 구하는 함수
	void	SetTendonArcType(CPlateBasicIndex *pBx);
	void	GetRegulationSteelMaterial(long nMaterial, CString szNameSteel, CString &szLeft, CString &szRight);


	void GetIndexGirderToCrossBeam(CLongArray &nArrGir, CPlateBasicIndex *pBx, BOOL bPartDim);
};

#endif // !defined(AFX_PLATEBRIDGEAPP_H__AA715BEF_2E84_440F_9D8B_306CD1349872__INCLUDED_)
