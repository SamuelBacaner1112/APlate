// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignStringer.h: interface for the CXLDesignStringer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNSTRINGER_H__631279A9_39BB_4D06_864E_66E0FEA978B2__INCLUDED_)
#define AFX_XLDESIGNSTRINGER_H__631279A9_39BB_4D06_864E_66E0FEA978B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLDesignStringer : public CAPlateOutCalcXL  
{
public:
	CXLDesignStringer(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignStringer();

	CXLControl			*m_pXL;
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};

	long m_nSttRow;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
};

#endif // !defined(AFX_XLDESIGNSTRINGER_H__631279A9_39BB_4D06_864E_66E0FEA978B2__INCLUDED_)
