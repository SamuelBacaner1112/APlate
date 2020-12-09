// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutTUGirder.h: interface for the CAPlateOutTUGirder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTTUGIRDER_H__6DD33B09_D062_4A07_B66B_BFA9C3843FC9__INCLUDED_)
#define AFX_APLATEOUTTUGIRDER_H__6DD33B09_D062_4A07_B66B_BFA9C3843FC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutTUGirder : public CDrawPage
{
public:
	CAPlateOutTUGirder();
	virtual ~CAPlateOutTUGirder();

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();

private:
	void AddPlan(CDomyun *pDom, long nSubPage);
	void AddJong(CDomyun *pDom, long nSubPage);
	void AddDetail(CDomyun *pDom, long nSubPage, BOOL bAnchor);
	void AddDetailSteel(CDomyun *pDom, long nSubPage);
	void AddTableSteel(CDomyun *pDom, long nSubPage);

	void InsertDomyun(CDomyun *pDom, long nSubPage);
};

#endif // !defined(AFX_APLATEOUTTUGIRDER_H__6DD33B09_D062_4A07_B66B_BFA9C3843FC9__INCLUDED_)
