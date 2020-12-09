// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLCheckListSection.h: interface for the CXLCheckListSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLCHECKLISTSECTION_H__DF1C9AF0_6FB1_435D_90C3_D2B0AB4B0231__INCLUDED_)
#define AFX_XLCHECKLISTSECTION_H__DF1C9AF0_6FB1_435D_90C3_D2B0AB4B0231__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLCheckListSection  
{
public:
	CXLCheckListSection(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLCheckListSection();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;

	void SectionSummary();
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void SectionCheck();
	void SectionUltimate();
	void SectionUltimate(long &nSttRow, long nG);
	void SectionSplice(long &nSttRow, long nG);
	void Splice();
	void Weld();

	void FilletWeldMeasureCheck();
	void WeldStressCheck();
	void SetLines(int nLine);
protected:
	long				m_nSttRow;
	CPlateBasicIndex	*m_pVStiffBx;
	long	m_nSectionSize;
	long	m_nG;
private:
	CXLControl *m_pXL;
};

#endif // !defined(AFX_XLCHECKLISTSECTION_H__DF1C9AF0_6FB1_435D_90C3_D2B0AB4B0231__INCLUDED_)
