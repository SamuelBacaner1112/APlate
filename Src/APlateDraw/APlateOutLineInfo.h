// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutLineInfo.h: interface for the CAPlateLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTLINEINFO_H__98085714_D48E_477C_846C_07B62EF4D81D__INCLUDED_)
#define AFX_APLATEOUTLINEINFO_H__98085714_D48E_477C_846C_07B62EF4D81D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutLineInfo : public CDrawPage  
{
public:
	CAPlateOutLineInfo();
	virtual ~CAPlateOutLineInfo();

 	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();
	
private:
	void AddLineInfoDiagram(CDomyun *pDom , long nLine);
	void AddJongdanDiagram(CDomyun *pDom  , long nLine);
	void AddPyungubaeDiagram(CDomyun *pDom, long nLine);

	void AddLineInfoTable(CDomyun *pDom, long nLine);
	void AddLineInfoTableIP(CDomyun *pDom, long nLine);
};

#endif // !defined(AFX_APLATEOUTLINEINFO_H__98085714_D48E_477C_846C_07B62EF4D81D__INCLUDED_)
