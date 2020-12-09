// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLCheckListFloor.h: interface for the CXLCheckListFloor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLCHECKLISTFLOOR_H__6235C12D_4913_4E71_AC82_DC37A45BFC6E__INCLUDED_)
#define AFX_XLCHECKLISTFLOOR_H__6235C12D_4913_4E71_AC82_DC37A45BFC6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLCheckListFloor  :public CAPlateOutCalcXL
{
public:
	CXLCheckListFloor(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLCheckListFloor();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void Floor();
	void With_Tendon_Floor();
	void Without_Tendon_Floor();
	void Reaction();
	void Torsion();
	
private:
	CXLControl	*m_pXL;
	void	MakeExperienceTable();
	double	GetRebarArea(long nRebarDia);
protected:
	long m_nSttRow;
	void SetLines(int nLine);
	double GetDeflection(long nPos, CString szLoad);
	void GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr);
	double GetAllowDeflection(long nPos);
	void SeekFlange_Web(double& dFlange, double& dWeb);

};

#endif // !defined(AFX_XLCHECKLISTFLOOR_H__6235C12D_4913_4E71_AC82_DC37A45BFC6E__INCLUDED_)
