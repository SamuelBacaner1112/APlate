// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLCheckListStud.h: interface for the CXLCheckListStud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLCHECKLISTSTUD_H__8A33E61C_6D3E_42AD_B142_64F9848405B0__INCLUDED_)
#define AFX_XLCHECKLISTSTUD_H__8A33E61C_6D3E_42AD_B142_64F9848405B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLCheckListStud  
{
public:
	CXLCheckListStud(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLCheckListStud();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;

	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void CheckStud();
	
private:
	long		m_nSttRow;
	CXLControl *m_pXL;		
};

#endif // !defined(AFX_XLCHECKLISTSTUD_H__8A33E61C_6D3E_42AD_B142_64F9848405B0__INCLUDED_)
