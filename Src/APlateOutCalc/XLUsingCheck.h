// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLUsingCheck.h: interface for the CXLUsingCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLUSINGCHECK_H__20850C6B_DD39_418F_8C32_A2A77EE57794__INCLUDED_)
#define AFX_XLUSINGCHECK_H__20850C6B_DD39_418F_8C32_A2A77EE57794__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLUsingCheck : public CAPlateOutCalcXL  
{
public:
	CXLUsingCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLUsingCheck();

	CXLControl			*m_pXL;
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};

	long m_nSttRow;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;

	void CheckFatigueStress();
	void CheckFatigueStressSub(long nChecksu);
	void CheckDroop();
};

#endif // !defined(AFX_XLUSINGCHECK_H__20850C6B_DD39_418F_8C32_A2A77EE57794__INCLUDED_)
