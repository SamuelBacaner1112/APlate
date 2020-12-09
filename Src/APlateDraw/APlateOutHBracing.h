// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutHBracing.h: interface for the CAPlateOutHBracing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTHBRACING_H__D732CEDA_5447_4196_B03C_ED46455BEB85__INCLUDED_)
#define AFX_APLATEOUTHBRACING_H__D732CEDA_5447_4196_B03C_ED46455BEB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef CTypedPtrArray	<CObArray, CHBracingGusset*> CHBracingGussetList;

class CAPlateOutHBracing : public CDrawPage  
{
private:
	long nSangseSu;
public:
	CAPlateOutHBracing();
	virtual ~CAPlateOutHBracing();

	void DrawOnePage();
	long GetPageSu();
	long GetPagesuGeneralTotal();
	double GetScale();
	CString GetTitle();
	CTypedPtrArray	<CObArray, CHBracingGusset*> m_HBracingGussetList;
	void MakeHBracingGussetList();
protected:
	CPlateBasicIndex* GetPageBxStt(long nG, long nSubPage, BOOL bFrontMatch=FALSE, BOOL bNextMatch=FALSE);
	CPlateBasicIndex* GetPageBxEnd(long nG, long nSubPage, BOOL bFrontMatch=FALSE, BOOL bNextMatch=FALSE);

	void AddPlan(CDomyun *pDom, long nSubPage);
	void AddPlanDetail(CDomyun *pDom, long nSubPage);

};

#endif // !defined(AFX_APLATEOUTHBRACING_H__D732CEDA_5447_4196_B03C_ED46455BEB85__INCLUDED_)
