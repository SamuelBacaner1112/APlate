// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignStiff.h: interface for the CXLDesignStiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNSTIFF_H__DE8FC61D_5B82_4D6F_92AF_E0CE6862745C__INCLUDED_)
#define AFX_XLDESIGNSTIFF_H__DE8FC61D_5B82_4D6F_92AF_E0CE6862745C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLDesignStiff : public CAPlateOutCalcXL  
{
public:
	CXLDesignStiff(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignStiff();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	///< 플랜지 웨브 보강재 검토	- 지점부, 일반부
	void WebStiff(CStringArray &strArrUseSheet);
	///< 지점부 보강재 검토			- 교대부, 교각부
	void JijumSupport(CStringArray &strArrUseSheet, BOOL bIsPier);
	///< 잭업 보강재 검토			- 교대부, 교각부
	void JackUpSupport(CStringArray &strArrUseSheet, BOOL bIsPier);

	// SHOE 1개소당 JACK UP장치 설치개소수 구하기
	long GetJackupBuildsuByOneShoe(CPlateBasicIndex *pBx);
protected:
	long				m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	void SetLines(int nLine);
	void WebStiff(CPlateBasicIndex *pBx, BOOL bUpper);
};

#endif // !defined(AFX_XLDESIGNSTIFF_H__DE8FC61D_5B82_4D6F_92AF_E0CE6862745C__INCLUDED_)
