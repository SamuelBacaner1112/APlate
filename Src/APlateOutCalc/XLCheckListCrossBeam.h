// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLCheckListCrossBeam.h: interface for the CXLCheckListCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLCHECKLISTCROSSBEAM_H__C1A8B90D_63E1_4F23_928E_72381CB3F7F3__INCLUDED_)
#define AFX_XLCHECKLISTCROSSBEAM_H__C1A8B90D_63E1_4F23_928E_72381CB3F7F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLCheckListCrossBeam  
{
public:
	CXLCheckListCrossBeam(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLCheckListCrossBeam();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;

	void CrossBeamGeneral(int nSeq);
	void CrossBeamGeneralDetail(int nSeq, CPlateBasicIndex *pBx, CString sGubun);
	void CorssBeamBracing(int nSeq);
	void CrossBeamBracingDetail(int nSeq, CPlateBasicIndex *pBx, CString sGubun);
	void HorBracing(int nSeq);
	BOOL HorBracingDetail(int nSeq, long nG, long nBr);
	void CheckListTitle(int nSeq);
	
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void CrossBeam();
	void CrossBeamHorVerBracing();
	void CrossBeamJewon();
	void CrossBeamMoment();
	void SetLines(int nLine);
	CString RetCrossBeamType(double sType, CPlateBasicIndex *pBx);
protected:
	long				m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	CString GetStrValue(double dValue);
};

#endif // !defined(AFX_XLCHECKLISTCROSSBEAM_H__C1A8B90D_63E1_4F23_928E_72381CB3F7F3__INCLUDED_)
