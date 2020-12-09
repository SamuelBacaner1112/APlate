// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLCheckListUsing.h: interface for the CXLCheckListUsing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLCHECKLISTUSING_H__0D2DC76F_3745_49DB_823C_B43159AC856E__INCLUDED_)
#define AFX_XLCHECKLISTUSING_H__0D2DC76F_3745_49DB_823C_B43159AC856E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLCheckListUsing  
{
public:
	CXLCheckListUsing(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLCheckListUsing();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;

	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void Fatigue();
	void ExpansionJoint();
	void OutExcelCheck11_Piro_Shear(long &nSttRow);
	void OutExcelCheck11_Deflection();	
	
protected:

	long m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
};

#endif // !defined(AFX_XLCHECKLISTUSING_H__0D2DC76F_3745_49DB_823C_B43159AC856E__INCLUDED_)
