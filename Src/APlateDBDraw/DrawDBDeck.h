// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawDBDeck.h: interface for the CDrawDBDeck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDBDECK_H__DE2165CE_3CC4_4B90_A185_460CA55D47FE__INCLUDED_)
#define AFX_DRAWDBDECK_H__DE2165CE_3CC4_4B90_A185_460CA55D47FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define COMMA2(d)	"("+COMMA(d)+")"	

#define MARK_BASEHEIGHT_UPPER	Dom.Always(MARK_BASE_HEIGHT)
#define MARK_BASEHEIGHT_LOWER	Dom.Always(MARK_BASE_HEIGHT+MARK_BASE_RADIUS*1.5)
#define LENGTH_JOINT		1200	// 주철근의 이음 길이
#define GAP_JOINT			30		// 주철근과 이음간의 간격

#define REBARPOS_UP			0
#define REBARPOS_DOWN		1	
#define REBARPOS_LEFT		2
#define REBARPOS_RIGHT		3
#define REBARPOS_DOWN_LEFT	4
#define REBARPOS_DOWN_RIGHT	5
#define REBARPOS_UP_LEFT	6
#define REBARPOS_UP_RIGHT	7
#define REBARPOS_NONE		8

#define REBAR_MAIN_LAPJOINT_GAP 15

typedef CSimpleTypedArray <CRebarJoint> CJointArray;

class CDataManage;
class AFX_EXT_CLASS CDrawDBDeck
{
public:
	CDrawDBDeck(CDataManage *pDataMng);
	virtual ~CDrawDBDeck();

private:
	CDataManage *m_pDataMng;  

public:
	// 블럭 관련
	CObInsert* InsertHCadBlock(CDomyun *pDomP, const CString& szBlkName, CDPoint xy, double dScale, double dRotDegAng=0, BOOL bHCADOnly=TRUE);
	CObInsert* InsertHCadBlock(CDomyun *pDomP, const CString& szBlkName, double x, double y, double dScale, double dRotDegAng=0)
	{
		return InsertHCadBlock(pDomP, szBlkName, CDPoint(x,y), dScale, dRotDegAng);	
	}
	void SetAttrib(CDomyun *pDom, CObInsert *pIns, const CString& szTag, const CString& szText);
	CDomyun m_HCadSymbolDom;	
	CStringArray *m_pBlockNameArray;
	CStringArray *m_pBlockUserNameArray;
	BOOL m_bUseHCadBlock;
	BOOL IsUseHCadBlock();
	void SetUseHCadBlock(BOOL vUseBlock);
	CARoadOptionStd* m_pARoadOptionStd;
	CDomyun* GetHCadSymbolDom();
	CString m_sMarkForMarkRebar;
	
	void MarkCenterOfRoad(CDomyun *pDomP, CDPoint xyText, double dLength=0.0, CString strSec=_T(""));
	// 그리기도구 함수
	void DrawConstJointHor(CDomyun* pDomP, CDPoint xy, double dWidth);
	void DrawConstJointVer(CDomyun* pDomP, CDPoint xy, double dHeight, BOOL bLeft);
	void DimRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, CDPoint xyStt, CDPoint xyEnd, BOOL bLeft, long nDimDan, long nDrawLeft=0, BOOL bAli=FALSE);
	void DrawRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawJointArray(CDomyun *pDomP, CJointArray &arrJoint, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawJointByRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC);
	void DrawJoint(CDomyun *pDomP, CRebarJoint &joint, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawCuttingTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine=TRUE);
	void DrawCuttingTvArrayHorRange(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine);
	void DrawRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawLineArray(CDomyun *pDomP, CDPointArray &xyArr);
	void AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDPointArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin=TRUE, BOOL bAli=FALSE, BOOL bInput=FALSE, BOOL bAutoModifyStt=FALSE);
	void AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDoubleArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin=TRUE);
	void DrawPointRebar(CDomyun *pDom, CDPoint xyMark, long nPos, double dDia=REBAR_SYMBOL_RADIUS);
	long DrawPointRebarByLine(CDomyun *pDom, CDPoint xyStt, CDPoint xyEnd, double dCTC, long nPos,
								double dEraseLengthFrStt=0, CDPoint& xyFirstRebarPos=CDPoint(0,0), CDPoint& xyLastRebarPos=CDPoint(0,0));

	void GetDimInfoAtRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bHor, CDPointArray &xyArrDim, CStringArray &sArrInfo, BOOL bMerge, BOOL bApplyJoint, BOOL bApplyEndIs2, BOOL bReverse=FALSE, double dCoverStt=0, double dCoverEnd=0);
	void DrawRebarShapeArr(CDomyun *pDom, vector<REBAR_SHAPE> &vShape, BOOL bInput, BOOL bShowShapeNo, long nCurShape);
	void DrawTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bDrawInfo=FALSE, BOOL bDrawDirInfo=FALSE, UINT nTypeDrawOverLapLine=0);
	// 그룹코드 관련
private:
	void GetDimInfoByTypeDim(CDPoint &xyStt, CDPoint &xyEnd, BOOL bHor, long nTypeDim, long nDrawLeft, double dStaCuttingOrg);
	long GetTypeDimRebarInfoCycle(BOOL bHor, CDPoint xyStt, CDPoint xyEnd, long nDrawLeft, double dStaCuttingOrg);
	void SetStd();
	long m_nCurGroupCode;		// 현재 그리는 객체의 그룹코드
	BOOL m_bSettingGroupCode;	// 그룹코드를 적용할지?

public:
	void MarkRadiusRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, CDRect rectLimit=CDRect(0, 0, 0, 0));
	
	// 도면 그릴때 넘겨받은 도면의 셋팅을 그대로 유지 시킬지? ///////
	// layer설정시 도면 셋팅이 초기화 되므로 엑셀출력시 기껏 설정해놓은 셋팅이 적용되지 않기때문에 
	// 옵션을 둠.
	BOOL m_bApplyDimSettingOfBaseDom;	
	void ApplyDimSetting(CDomyun *pDomPSource, CDomyun *pDomPDest);
	/////////////////////////////////////////////////////////////////
	void GetTvArrTrimByPolygon(CTwinVectorArray &tvArrSource, CTwinVectorArray &tvArrPolygon, BOOL bOutside, CTwinVectorArray &tvArrResult);
	BOOL IsMarkRebar(CString sMark);
	void SetMarkForMarkRebar(CString sMark);
	void DrawTvArrayWidthDashDot(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bHiddenAtStt);
	void DrawTvArrWidthRound(CDomyun *pDomP, CTwinVectorArray &tvArr, double dLenRound, BOOL bRoundAtStt);
	void ModifyInfoAutoDimConc(CStringArray &sArrText, CStringArray &sArrUnderText, BOOL bInput);
	BOOL IsDimByAngleLen();
	CDRect MergeRectAndRect(CDRect rect1, CDRect rect2);
	void GetDimInfoAtDistArray(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CDoubleArray &dArrDist, CString sMark, BOOL bInput, BOOL bMerge, BOOL bHor=TRUE);
	double GetStaCuttingOrg();
	CString GetStringEL(double dEL);
	void GetDimInfoAtPlaceRebarByBlock(CDPointArray &xyArr, CDPointArray &xyArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, CDPoint xyStt, CDPoint xyDir,  BOOL bInput, BOOL bMerge);
	void GetDimInfoAtPlaceRebarByBlock(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, BOOL bInput, BOOL bMerge);
	BOOL IsDrawByLineInfo(long nPosInRebarInfoCycle);
	void MarkRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nTypeStt=2, long nTypeEnd=0, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), double dScalePos=0.5);
	void MarkRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nType, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), double dScalePos=0.5);
	void DrawCuttingTvArraySingle(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rcCut, BOOL bDrawInfo=FALSE);
	void SettingCurGroupCode(BOOL bVal);
	void InitCurGroupCode();
	
};

#endif // !defined(AFX_DRAWDBDECK_H__DE2165CE_3CC4_4B90_A185_460CA55D47FE__INCLUDED_)
