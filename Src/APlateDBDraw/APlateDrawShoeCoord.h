// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawShoeCoord.h: interface for the CAPlateDrawShoeCoord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWSHOECOORD_H__CB4203ED_8336_4F09_9780_E5E8CE6D7518__INCLUDED_)
#define AFX_APLATEDRAWSHOECOORD_H__CB4203ED_8336_4F09_9780_E5E8CE6D7518__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CDataManage;
class AFX_EXT_CLASS CAPlateDrawShoeCoord  
{
public:
	CAPlateDrawShoeCoord(CDataManage *pDataMng);
	virtual ~CAPlateDrawShoeCoord();

private:
	CDataManage *m_pDataMng;

public:
	void DrawPierElevationTitle(CDomyun *pDom, CDPoint Base, double dRowHeight);
	void DrawPierElevationTable(CDomyun *pDom, long nPier, CDPoint Base, double dRowHeight);

};

#endif // !defined(AFX_APLATEDRAWSHOECOORD_H__CB4203ED_8336_4F09_9780_E5E8CE6D7518__INCLUDED_)
