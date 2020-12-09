// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLCheckListStiff.h: interface for the CXLCheckListStiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLCHECKLISTSTIFF_H__522000EF_345A_4CD1_B886_CE02BC4495BE__INCLUDED_)
#define AFX_XLCHECKLISTSTIFF_H__522000EF_345A_4CD1_B886_CE02BC4495BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLCheckListStiff  
{
public:
	CXLCheckListStiff(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLCheckListStiff();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;

	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void Stiff();
	void HStiff();
	void VStiff();
	void JijumStiff();

	
protected:
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	long				m_nSttRow;
	void SetLines(int nLine);
};

#endif // !defined(AFX_XLCHECKLISTSTIFF_H__522000EF_345A_4CD1_B886_CE02BC4495BE__INCLUDED_)
