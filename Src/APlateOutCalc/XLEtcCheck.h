// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLEtcCheck.h: interface for the CXLEtcCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLETCCHECK_H__BFAC33E1_91B0_4F48_B812_A7D0B6CE08D5__INCLUDED_)
#define AFX_XLETCCHECK_H__BFAC33E1_91B0_4F48_B812_A7D0B6CE08D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLEtcCheck : public CAPlateOutCalcXL  
{
public:
	CXLEtcCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLEtcCheck();

	CXLControl			*m_pXL;
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};

	long m_nSttRow;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;

	void CheckCamber();
	void CheckExpasionJoint(long nMethod);
};

#endif // !defined(AFX_XLETCCHECK_H__BFAC33E1_91B0_4F48_B812_A7D0B6CE08D5__INCLUDED_)
