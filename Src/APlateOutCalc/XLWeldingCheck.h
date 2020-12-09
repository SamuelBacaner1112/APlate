// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLWeldingCheck.h: interface for the CXLWeldingCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLWELDINGCHECK_H__C2DD69B2_1E7F_4668_821B_42B13F776E0D__INCLUDED_)
#define AFX_XLWELDINGCHECK_H__C2DD69B2_1E7F_4668_821B_42B13F776E0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLWeldingCheck : public CAPlateOutCalcXL  
{
public:
	CXLWeldingCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLWeldingCheck();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void	SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void	WeldingCheck();
	double	GetMaxReaction(BOOL bPier);
	long	GetMaxJoint(BOOL bPier);
	BOOL	ExistAbut()	{return m_ArrBxAbut.GetSize() > 0 ? TRUE : FALSE;	}
	BOOL	ExistPier()	{return m_ArrBxPier.GetSize() > 0 ? TRUE : FALSE;	}
protected:
	long	m_nSttRow;
	long	m_nSectionSize;
	long	m_nG;
	long	m_nMaxJoint;
	double	m_dMaxReaction;
	CPlateBasicIndex	*m_pVStiffBx;
	CTypedPtrArray < CObArray, CPlateBasicIndex* > m_ArrBxAbut;
	CTypedPtrArray < CObArray, CPlateBasicIndex* > m_ArrBxPier;

	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	void SetLines(int nLine);
	void FilletWeldMeasureCheck();			///< 필렛용접 치수 검토
	void WeldStressCheck();					///< 용접부의 응력 검토
	void WeldVStiffCheck();					///< 수직보강재 용접부 검토
	void ReinforceWeldCheck(BOOL bIsPier);	///< 지점부 보강재 용접부 검토

	void GetJijumVStiffBxByMaxReaction(BOOL bPier);
};

#endif // !defined(AFX_XLWELDINGCHECK_H__C2DD69B2_1E7F_4668_821B_42B13F776E0D__INCLUDED_)
