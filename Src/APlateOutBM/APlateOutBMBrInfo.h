// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutBMBrInfo.h: interface for the CAPlateOutBMBrInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTBMBRINFO_H__2877833C_845B_4337_9B1A_5664688EF622__INCLUDED_)
#define AFX_APLATEOUTBMBRINFO_H__2877833C_845B_4337_9B1A_5664688EF622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SECTION_DANBU_STT	0
#define SECTION_DANBU_END	1
#define SECTION_GEN			2
#define SECTION_JIJUM		3

class CPlateBasicIndex;
//----------class CBrBeam;

class CAPlateOutBMBrInfo  
{
public:
	CAPlateOutBMBrInfo(CAPlateOutBMStd *pAPlateOutBMStd);
	virtual ~CAPlateOutBMBrInfo();

	CAPlateOutBMStd	*m_pAPlateOutBMStd;
	CString	m_szBridgeName;			// 교량파일 이름
	CString m_szBridgePath;			// 교량파일 경로
	CString m_szBridgeDir;			// 교량 진행방향
	BOOL	m_bOutTotSlab;			// 슬래브수량총괄 출력여부
	BOOL	m_bOutTotRebar;			// 철근수량총괄 출력여부
	BOOL	m_bOutRebar;			// 철근 집계표 출력여부
	BOOL	m_bOutSlab;				// 슬래브 수량 출력여부
	BOOL	m_bOutCalcFoundation;	// 산출근거
	long	m_nSpan;				// 

	void GetSlabHunch(CPlateBasicIndex *pBx, CVectorArray &ptArr);
	void GetGirderWidth(long &nGirdersu1, double &dGirderWidth1, long &nGirdersu2, double &dGirderWidth2);
	//----------double GetBrBeamWidth(CBrBeam *pBrBeam);
	double GetCantileverWidth(BOOL bLeft);
	// double GetHeightCrossBeamHunch(BOOL bMaxHeight);
	void GetCrossBeamHunch(double &dMaxLength, double &dMinLength, long &nQtyHunch, 
								double &dMinHeight, double &dMaxHeight, double &dLowWidthdouble);
// 	double GetWidthCrossBeamLow();
// 	long GetQtyCrossBeamHunch();			
	double GetGirderHeight(BOOL bMax);
	void GetSlabVerticalDimensions(CVectorArray &ptArr);
	double GetUpperLengthAver(BOOL bPositive);
	double GetConfinementLengthAver();

	void DrawSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawSlabDongbari(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawMainDomyun(CDomyun *pDom, long nSectionPos);
	void DrawSlabHunchGen(CDomyun *pDom);
	void DrawSlabFrontGen(CDomyun *pDom);
	void DrawCrossRebarBindConc(CDomyun *pDom);
	CPlateBasicIndex* GetBxSection(long nSectionPos);
	double GetBridgeLength(long nGuardFence);

	double GetLengthGirder(long nG);
	CDoubleArray GetSizeConnectPart(long nG, BOOL bUpper);		// 이음부 개수 0:인장부 1:교번부 2:압축부
	CDoubleArray GetLengthWebButtWelding();						// 복부판 맞대기이음 길이(거더별)
	//CDoubleArray GetLengthJongRib(BOOL bUpper);					// 종리브 길이(거더별)
	CDoubleArray GetLengthHStiffener();							// 수평보강재 길이(거더별)
	//CDoubleArray GetSizeJijumStiffener(long nG);				// 지점보강재 개수(지점별)
	void GetLengthJijumStiffener(long nG, CDoubleArray &arrLength, CDoubleArray &arrSize);				// 지점부강재 길이(지점별)
	//double GetLengthBracketUp(CString szTarget);				// 가로보(지점부,일반부), 외측가로보(지점부,일반부), 세로보 브라켓상판 길이

	//상부/하부  플랜지 폭원 및 모멘트 구간.....
	void GetFlangeLengthCompress(BOOL bUpper, CDoubleArray &arrFlangeWidth, 
		CDoubleArray &arrCompress, CDoubleArray &arrSort, double dCompress[3][99]);
};

#endif // !defined(AFX_APLATEOUTBMBRINFO_H__2877833C_845B_4337_9B1A_5664688EF622__INCLUDED_)