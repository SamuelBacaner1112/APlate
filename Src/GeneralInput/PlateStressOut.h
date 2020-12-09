// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateStressOut.h: interface for the CPlateStressOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATESTRESSOUT_H__3DDF1F3B_A3EB_4F12_9E63_958F5A0AA4D1__INCLUDED_)
#define AFX_PLATESTRESSOUT_H__3DDF1F3B_A3EB_4F12_9E63_958F5A0AA4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDataManage;
class CPlateBridgeApp;
class CLineInfoApp;
class CAPlateCalcStd;
class CFEMDraw;
class CFTextOutDlg;
class CCrossBeamVerStiff;
class CCrossBeamHorStiff;
class CShRichEdit;

class AFX_EXT_CLASS CPlateStressOut
{
public:

	CPlateStressOut(CGeneralBaseStd *pCalcStd);
	virtual ~CPlateStressOut();

	// Slab단면해석
	void SlabSectionOutput(CStringArray &Arr);                        
	void SlabRebarOutput(CStringArray &Arr, long nPos);

	///< 프리스트레스의 손실
	void SlabSectionPrestress(long nSide, CShRichEdit &RichEdit);
	void SlabPrestresscheck(long nSide, CShRichEdit &RichEdit);		///< 바닥판 슬래브 응력검토

	void SlabSectionOutputLeft(CStringArray &Arr);
	void SlabSectionOutputRight(CStringArray &Arr);
	void SlabSectionOutputCenter(CStringArray &Arr);
	void SlabSectionOutputReview(CStringArray &Arr);
	//사하중
	void Output_DeadLoad(CStringArray &Arr, long nPos);					//nPos=0 : 좌측캔틸레버, nPos=1 : 우측캔틸레버 nPos=2 : 중앙부슬래브
	void Output_MoveLoad(BOOL bLeft, CShRichEdit &RichEdit);			// 활하중
	void Output_MoveLoadCenter(CShRichEdit &RichEdit);		            // 활하중(중앙부)
	void Output_CollisionLoad(long nSide, CShRichEdit &RichEdit);		// 충돌하중
	void Output_WindLoad(long nSide, CShRichEdit &RichEdit);			// 풍하중
	void Output_CentriodLoad(long nSide, CShRichEdit &RichEdit);		// 원심하중
	void Output_TotalCombinationLoad(long nPos, CShRichEdit &RichEdit);		// 하중조함
	void Output_SlabThickness(long nPos, CShRichEdit &RichEdit);		// 슬래브최소두께검토 nPos=0 : 좌측캔틸레버, nPos=1 : 우측캔틸레버 nPos=2 : 중앙부슬래브
	void Output_SlabThickness_Tendon(CShRichEdit &RichEdit);			// 강선이 있을 경우 바닥판 최소 두께 설정
	void Output_RebarQtyCheck(long nPos, CShRichEdit &RichEdit, long nCheckType);           //슬래브철근량검토
	void Output_DroopReview(long nSide, CShRichEdit &RichEdit);		      //처짐검토    
	void Output_CrackReview(long nSide, CShRichEdit &RichEdit);		       //균열검토
	void Output_GiJumReview(CStringArray &Arr);					       //지점검토
	void OutPut_CentriodLoadCenter(CShRichEdit &RichEdit);						// 슬래브중앙부 원심하중
	void Output_InnerFloor(CShRichEdit &RichEdit);
	void Output_RebarQtyExperienceCheck(CShRichEdit &RichEdit);			// 중앙부 경험적 설계
	void Output_UltimateLoad(long nSide, CShRichEdit &RichEdit);		// 소수주형 극한하중 검토
	void Output_UltimateLoadCheck(long nSide, CShRichEdit &RichEdit);	// 극한하중에 대한 단면검토 (2009.07.24)
	void Output_FixingPlate(CShRichEdit &RichEdit);						// 정착부 보강 검토
	void Output_WeldReview(long nType, CShRichEdit &RichEdit, BOOL bJijum); //용접 검토
	// 현장이음검토
	void SpliceCheckOutput(CStringArray &Arr, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceCheckOutput(CStringArray &Arr, long nG, long nSp, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceFlangeCheckUp(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Sf, double Sa, double Ra, double Dh, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceFlangeCheckLo(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Sf, double Sa, double Ra, double Dh, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceWebCheck(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Ms, double Mv, double Su, double Sl, double Sa, double Ra, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void MakeIoTable (CStringArray &Arr, double pTable [][8], int nCount, int nLMargin = 5);
	// 단면응력검토
	void StressCheckCoefOutput(CStringArray &Arr, long nG, double dSta, BOOL bDistance);
	// 가로보 검토
	void SectionCheckOutput(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckVBracingGenOutput(CStringArray &Arr, CPlateBasicIndex *pBx); // 수직브레이싱(일반타입)
	void StressCheckVBracingJijumOutput(CStringArray &Arr, CPlateBasicIndex *pBx); // 수직브레이싱(지점타입)
	void StressCheckEndCrossBeamGeneralType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckEndCrossBeamVBracingType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckCenterCrossBeamGeneralType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckCenterCrossBeamVBracingType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);

	// Modeling결과
	void ElementForceOutput(CStringArray &Arr);                        // 부재력 집계
	void NodeReactionOutput(CStringArray &Arr);                        // 반력 집계
	void NodeDisplaceOutput(CStringArray &Arr);                        // 처짐 집계
	void NodeCamberOutput(CStringArray &Arr);						   // 솟음집계	
	void Output_NodeDisplaceCheck(CStringArray &Arr);                  // 처짐검토

	///< 수직보강재의 검토
	void CheckVStiff(CStringArray &Arr, CPlateBasicIndex *pBx);
	CGeneralBaseStd *m_pStd;	
	double GetMapValue(CString szMapStr);

	void VerStiff(CStringArray &Arr, CCrossBeamVerStiff *pVerStiff, long nQtyHorStiff);
	void HorStiff(CStringArray &Arr, CCrossBeamHorStiff *pHorStiff);

	void FillOnOKorNG(CShRichEdit &RichEdit, BOOL bIsOK, long nCnt);
	// 계산에 필요한 플랜지 두께와 웹 두께 산정
	void SeekFlange_Web(double& dFlange, double& dWeb);

};

#endif // !defined(AFX_PLATESTRESSOUT_H__3DDF1F3B_A3EB_4F12_9E63_958F5A0AA4D1__INCLUDED_)
