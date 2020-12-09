// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutCamber.h: interface for the CAPlateOutCamber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTCAMBER_H__5C72F4C7_AD79_49E0_A5EA_4D20B510366F__INCLUDED_)
#define AFX_APLATEOUTCAMBER_H__5C72F4C7_AD79_49E0_A5EA_4D20B510366F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutCamber : public CDrawPage  
{
public:
	CAPlateOutCamber();
	virtual ~CAPlateOutCamber();

// Attributes
protected:
	double	m_YRatio;	//캠버 기울기
	double	m_YRatio2;	//기준선 기울기
	long	m_nG;		//현재 거더
	double	m_SttSta;	//시작 스테이션
	double	m_EndSta;	//끝 스테이션
public:	
	long	m_nDWSStyle;		//출력 스타일 1:대우 2:대명
//	long	m_nPagePerJigan;	//페이지당 출력될 지간수

public:
	void DrawOnePage() { AddOnePage(); };

// Implementaion
protected:
	CString GetTitle();

	void DrawYSymbol(CDomyun *pDomP);

	//횡단면도
	void DimGirderDist(CDomyun *pDomP,CPlateBasicIndex *pBx);
	void DimSlabThick(CDomyun *pDomP,CPlateBasicIndex *pBx);
	void DimTextEtc(CDomyun *pDomP,CPlateBasicIndex *pBx);
	//켬버도 관련
	void TextOutMultiLine(CDomyun *pDom, double x, double y, CString &cs, long nVertAlign = 0);
	void GetCamberStation(CVectorArray &staArr, long nSectionType, long nG);
	void SetPageData();
	double GetMaxCamberInGirder();

	//제작도 타입1
	void DrawAllCamberFact1(CDomyun *pDomP, CDPoint Base);
	void DrawDWSCamberDiagram(CDomyun *pDomP, CDPoint Base);
	void DrawDWSCamberDim(CDomyun *pDomP, CDPoint Base);
	void DrawDWSCamberTable(CDomyun *pDomP, CDPoint Base);

	//제작도 타입2
	void DrawAllCamberFact2(CDomyun *pDomP, CDPoint Base);			//전체호출
	void DrawDMCamberUppdrDim(CDomyun *pDomP, CDPoint Base);	//상단의 치수
	void DrawDMCamberTable(CDomyun *pDomP, CDPoint Base);		//본문테이블	
	void AddBumlaeMark(CDomyun *pDomP, CDPoint Base);

	//설계도 타입
	void DrawAllCamberDesign(CDomyun *pDomP, CDPoint Base);
	void DrawCamberDesign(CDomyun *pDomP, CDPoint Base);
	CDPoint DrawCamberDesignTable(CDomyun *pDomP, CDPoint Base, long nG);
	void DrawCamberDesignLegend(CDomyun *pDomP, CDPoint Base);
	//대명 타입 테이블 그리는 함수
	void SetDMTableTitleArr(CStringArray &arrTitle);
	//부차적인 함수
	BOOL IsSpot(CVector vSta);	//지점인지 아닌지...

	//소제목 쓰기
	void AddSmallTitle(CDomyun *pDomP);

public:
	void AddOnePage();
	long GetPageSu();
	CString GetPageTitle(long nPage, BOOL bShort);
	double GetScale();

};

#endif // !defined(AFX_APLATEOUTCAMBER_H__5C72F4C7_AD79_49E0_A5EA_4D20B510366F__INCLUDED_)
