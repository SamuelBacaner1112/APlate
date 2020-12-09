// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutSummary.h: interface for the CAPlateOutSummary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTSUMMARY_H__A00F1D74_B90C_40A1_9BE9_8A9849F011D0__INCLUDED_)
#define AFX_APLATEOUTSUMMARY_H__A00F1D74_B90C_40A1_9BE9_8A9849F011D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutSummary   : public CDrawPage  
{
public:
	CAPlateOutSummary();
	virtual ~CAPlateOutSummary();
	void DrawOnePage(); 
	long GetPageSu();
	CString GetTitle();
	double GetScale();

private:
	CPlateBasicIndex* GetBxSttPage(long nPage, long nG);
	CPlateBasicIndex* GetBxEndPage(long nPage, long nG);	
	void DrawStressOfMember(CDomyun *pDom, long nG, double dMomentY, double dScale, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawSectionSummary(CDomyun *pDom, long nG, double dMomentY, double dScale, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);

private:
	CPlateBasicIndex* m_pBxStt;
	CPlateBasicIndex* m_pBxEnd;
	long	m_nG;

};

#endif // !defined(AFX_APLATEOUTSUMMARY_H__A00F1D74_B90C_40A1_9BE9_8A9849F011D0__INCLUDED_)
