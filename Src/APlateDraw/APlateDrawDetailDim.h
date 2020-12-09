// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawDetailDim.h: interface for the CAPlateDrawDetailDim class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWDETAILDIM_H__1E005BBD_0713_4623_AC4B_7EEE5CCC9A72__INCLUDED_)
#define AFX_APLATEDRAWDETAILDIM_H__1E005BBD_0713_4623_AC4B_7EEE5CCC9A72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define SIDE_LEFT			-1
#define SIDE_CENTER			 0
#define SIDE_RIGHT			 1

#include "DimArray.h"

class CDataManage;
class CAPlateDrawStd;
class AFX_EXT_CLASS CAPlateDrawDetailDim  
{
public:
	CAPlateDrawDetailDim(CAPlateDrawStd *pStd, CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	virtual ~CAPlateDrawDetailDim();

private:
	CDomyun *m_pDom;	
	CPlateBasicIndex *m_pBxStt;
	CPlateBasicIndex *m_pBxEnd;
	CAPlateDrawStd *m_pStd;

	double long m_PyunDimFlag;
	double long m_JongDimFlag;
	double m_dLen_Title;
	CDPointArray m_JweonInfoStaArr;		//x : 시작 스테이션 y: 끝 스테이션	

public:	
	enum  DIMTYPE { DET_DIM_LEFT, DET_DIM_RIGHT, DET_DIM_CENTER, DET_DIM_BOTH };			
	
	void SetPyungDimDan(long nDan, BOOL bLeft,  BOOL bSlabDim=FALSE);
	void SetJongDimDan (long nDan, BOOL bUpper, BOOL bSlabDim=FALSE);
	void SetDirDimension(long nDir) {m_nDirDimension=nDir;}
	void DimTotal(CDomyun *pDomP, __int64 nType, BOOL bUpper, BOOL bLeft, BOOL bPyung, BOOL bGen=TRUE, BOOL bStartPage=FALSE, BOOL bLastPage=FALSE, BOOL bGirderNumber=FALSE);

public:				
	// 종단부재 제원표시
	void DimTextJongTotal (CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimTextJongForce(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimTextJongHStiff(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimTextJongHStiffOne(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxStt, double dStdSta);
	void DimTextJongWeb(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void TextOutJewonAndMat(CDomyun *pDomP, CDPoint xy,CString strBuje, CString strMat, BOOL bDirForward=TRUE);
	BOOL CheckStation(double sta1, double sta2);

	// 평면도
	void DimDetailSttPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt);
	void DimDetailEndPyung(CDomyun *pDom, CPlateBasicIndex *pBxEnd);			

	// 시종점 절단부 마크
	void DimDetailMLMarkPyung(CDomyun *pDom, long nNum, BOOL bUpper, long nUpperDan=-1, long nLowerDan=-1);
	
	// 종단면도
	void DimDetailSttJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bRotate = FALSE, BOOL bApplyJongdan = TRUE, long nDir = 0, BOOL bAuto = TRUE);
	void DimDetailEndJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bRotate = FALSE, BOOL bApplyJongdan = TRUE, long nDir = 0);						
	void DimJongSttEndSideBx(CDomyun* pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bBothSide=TRUE);
	void DimJongTaperBx     (CDomyun* pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimJongSectionMark	(CDomyun *pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bSlabBase=FALSE, double dOffsetHeight=0, BOOL bSameSize=FALSE);
	void DimJongJackUpMark	(CDomyun* pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimJongByundanFactor(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	
	// 시종점 절단부 마크
	void DimDetailMLMarkJong(CDomyun *pDom, long nNum, long nDir);	

	long GetPyungCurUpperDimDan() { return m_nUpperDan; }
	long GetPyungCurLowerDimDan() { return m_nLowerDan; }

	long GetJongCurUpperDimDan() { return m_nWebUpperDan; }
	long GetJongCurLowerDimDan() { return m_nWebLowerDan; }

private:
	// 평면도
	double GetStaFlangeByBx(CPlateBasicIndex *pBx, BOOL bUpper, long nDir, BOOL bApplyEdge = FALSE);	
	double GetDisFlangeBySta(CPlateGirderApp *pGir, double SttSta, double EndSta, BOOL bUpper, long nDir);
	void DrawDimPyungBx(CDomyun *pDom, DimDataArr* pDimData, long nDir, BOOL bUpper, BOOL bLeftPos, BOOL bGen, BOOL bTextThick=FALSE, BOOL bStartPage = FALSE, BOOL bLastPage=FALSE);

	// 측면도
	double GetStaWebByBx(CPlateBasicIndex *pBx, long nDir);
	double GetDisWebBySta(CPlateGirderApp *pGir, double SttSta, double EndSta, long nDir);	
	void DrawDimJongBx(CDomyun *pDom, DimDataArr* pDimData, long nDir, BOOL bUpper, BOOL bStud = FALSE, BOOL bTextThick=FALSE, BOOL bStartPage=FALSE, BOOL bLastPage=FALSE);	
	
	// Bx Make 함수
	void MakeStudBx(DimDataArr* DimData);	
	void MakeVStiffBx(DimDataArr* DimData, long nDir);	
	void MakeCrossBeamBx(DimDataArr* DimData, long nDir);
	void MakeFieldSpliceBx(DimDataArr* DimData, long nDir);
	void MakeFlangeBx(DimDataArr* DimData, long nDir, BOOL bUpper);
	void MakeWebBx(DimDataArr* DimData);
	void MakeFlangeSpliceBx(DimDataArr* DimData, long nDir, BOOL bUpper);
	void MakeWebSpliceBx(DimDataArr* DimData);
	void MakeByunBx(DimDataArr* DimData, long nDir, BOOL bUpper);
	void MakeTendonBx(DimDataArr* DimData, long nDir);
	void MakeJiGanBx(DimDataArr* DimData, long nDir);
	void MakeTotalBx(DimDataArr* DimData, long nDir);	

	//////////// 아래 두개는 임시적임
	void MakeBendingDimPyungBx(CPlateBasicIndex *pBxStt,CPlateBasicIndex *pBxEnd,DimDataArr* DimData, long nDir, BOOL bUpper);
	void MakeLowerBendingDimPyungBx(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, DimDataArr* DimData, long nDir);
		
private:
	CString m_DimTitle;
	long m_nWebUpperDan;
	long m_nWebLowerDan;

	long m_nUpperDan;
	long m_nLowerDan;
	BOOL m_bSlabDim;

	long m_nDirDimension;  // 치수선의 방향
};

#endif // !defined(AFX_APLATEDRAWDETAILDIM_H__1E005BBD_0713_4623_AC4B_7EEE5CCC9A72__INCLUDED_)
