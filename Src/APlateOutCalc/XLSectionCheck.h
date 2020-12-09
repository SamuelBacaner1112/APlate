// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLSectionCheck.h: interface for the CXLSectionCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLSECTIONCHECK_H__30E14625_D9C7_435B_BFED_DCECD62DDAFE__INCLUDED_)
#define AFX_XLSECTIONCHECK_H__30E14625_D9C7_435B_BFED_DCECD62DDAFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CXLSectionCheck  : public CAPlateOutCalcXL
{
public:
	CXLSectionCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLSectionCheck();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	enum { POSITIVE_SEC=0, NEGATIVE_SEC=1, POSITIVE_SP=2, NEGATIVE_SP=3, POSITIVE_SECPLUS=4, NEGATIVE_SECPLUS=5, POSITIVE_BUYNPLUS=6, NEGATIVE_BUYNPLUS=7, POSITIVE_USERPLUS=8, NEGATIVE_USERPLUS=9};
	void SectionCheck(long nPosType, CStringArray &strArrUsedSheet);
	long SectionUserCheck(long nG, double dSta);
	void SectionOutLine();
	void GetSectionCheckData(CDWordArray &dArrMat, CDWordArray &dArrNG, CDDWordArray &dArrSta, long nPosType, BOOL bPositive);
	CStringArray		m_ArrNotUseSheet;

protected:
	long	m_nSttRow;
	long	m_nPosType;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	CStringArray		m_ArrNumbering;

	void SectionCheck_TUGirder(long nPosType, CStringArray &strArrUsedSheet);
	void SectionCheck_GenGirder(long nPosType, CStringArray &strArrUsedSheet);
	void SetLines(int nLine);
	void SectionResources(long nMatNo, long nG, double dSta, long nPosType);			///< 1) 단면가정 및 설계조건
	void PositiveSection(long nG, double dSta, double dStaCheck);	///< 정모멘트	
	void NegativeSection(long nG, double dSta, double dStaCheck);	///< 부모멘트
	void SafetyTest(BOOL bPositive, BOOL bWind, BOOL bHybrid, double dYieldPointUp, double dYieldPointDn, double dFy=0/*부모멘트에서 사용*/);

	void PositiveSection_TUGirder(long nMatNo, long nG, double dSta, long nPosType);
	void NegativeSection_TUGirder(long nMatNo, long nG, double dSta, long nPosType);

	void GetSectionGirderClassify(CDWordArray &dArrMat, CDWordArray &dArrNG, CDDWordArray &dArrSta, long nPosType, BOOL bPositive);
	void GetSectionGirderNotClassify(CDWordArray &dArrMat, CDWordArray &dArrNG, CDDWordArray &dArrSta, long nPosType, BOOL bPositive);

	// TU연속교 부모멘트 dll 조정작업
	void SetTUContinousNegativeDLL();
};

#endif // !defined(AFX_XLSECTIONCHECK_H__30E14625_D9C7_435B_BFED_DCECD62DDAFE__INCLUDED_)
