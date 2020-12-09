// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutTendon.h: interface for the CAPlateOutTendon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTTENDON_H__3B42ABEF_7C57_468D_AE36_862F6A0FED5F__INCLUDED_)
#define AFX_APLATEOUTTENDON_H__3B42ABEF_7C57_468D_AE36_862F6A0FED5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDimDomyunEx;
class CAPlateOutTendon : public CDrawPage  
{
public:
	CAPlateOutTendon();
	virtual ~CAPlateOutTendon();

	void DrawOnePage(); 
	long GetPageSu();
	CString GetTitle();
	double GetScale();
	double m_dLastY; 
	CDRect m_rectDgk;

private:
	CPlateBasicIndex *GetBxSttPage(long nSubPage, long nG, BOOL bSttSlab = TRUE);
	CPlateBasicIndex *GetBxEndPage(long nSubPage, long nG, BOOL bEndSlab = TRUE);
	CPlateBasicIndex *m_pBxStt;
	CPlateBasicIndex *m_pBxEnd;
	long	m_nG;
	CStringArray m_sNoteArr;

	void AddPlan(CDomyun *pDom, long nSubPage);
	void AddDimPlan(CDomyun *pDom, long nSubPage);
	void DrawSlabByPage(CDomyun *pDomP, long nSubPage);
	void DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDan);
	void AddPlanGirderLine(CDomyun *pDom, long nSubPage);
	void DrawCenterLineAndMark(CDomyun *pDomP, long subPage);
	void InitNote();
	void AddNote(CDomyun *pDomP, long nSubPage);
	void DimNoteTypeVertical(CDomyun *pDomP);
	void AddFrontAA(CDomyun *pDomP, long nSubPage);
	void InsertDomyun(CDomyun *pDom, long nSubPage);
	void ReplaceData(CDomyun *pDom);
	void AddTable(CDomyun *pDom);
};

#endif // !defined(AFX_APLATEOUTTENDON_H__3B42ABEF_7C57_468D_AE36_862F6A0FED5F__INCLUDED_)
