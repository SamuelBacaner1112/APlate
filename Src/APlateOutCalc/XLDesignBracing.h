// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignBracing.h: interface for the CXLDesignBracing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNBRACING_H__F3A48DB1_248D_4180_8497_FFBB58E65F99__INCLUDED_)
#define AFX_XLDESIGNBRACING_H__F3A48DB1_248D_4180_8497_FFBB58E65F99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutCalcStd;
class CAPlateOutCalcXL;

class AFX_EXT_CLASS CXLDesignBracing : public CAPlateOutCalcXL  
{
public:
	CXLDesignBracing(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignBracing();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void DesignVBracing();
	void DesignHBracing();

protected:
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	CXLControl			*m_pXL;
	double				m_dPw;
	long				m_nSttRow;
	void SetLines(int nLine);
	void DesignVBracing(CPlateBasicIndex *pBx, long nIndex);
	void DesignHBracing(CPlateBasicIndex *pBx, long nIndex);
	void CheckGussetPlate(CPlateBasicIndex *pBx, long nG);
};

#endif // !defined(AFX_XLDESIGNBRACING_H__F3A48DB1_248D_4180_8497_FFBB58E65F99__INCLUDED_)
