// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLSpliceCheck.h: interface for the CXLSpliceCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLSPLICECHECK_H__A23DD200_1D0E_4AC8_995F_FA750EBB6165__INCLUDED_)
#define AFX_XLSPLICECHECK_H__A23DD200_1D0E_4AC8_995F_FA750EBB6165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLSpliceCheck : public CAPlateOutCalcXL  
{
public:
	CXLSpliceCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLSpliceCheck();

	CXLControl			*m_pXL;
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};

	long m_nSttRow;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;

	void SpliceCheck();
	void SpliceCheckCompressPart(CPlateBasicIndex *pBx);
	void SpliceCheckTensionPart (CPlateBasicIndex *pBx);
	void SpliceCheckWebPart1    (CPlateBasicIndex* pBx);
	void SpliceCheckWebPart2    (CPlateBasicIndex* pBx);
	void SetLines(int nLine);
	void SpliceWebPlanStree(CDomyun *pDom, CPlateBasicIndex* pBx, double dWidth);

};

#endif // !defined(AFX_XLSPLICECHECK_H__A23DD200_1D0E_4AC8_995F_FA750EBB6165__INCLUDED_)
