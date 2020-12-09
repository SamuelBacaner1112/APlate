// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawAbut.h: interface for the CAPlateDrawAbut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWABUT_H__F87B4FBD_5DA1_485A_949A_266BC4C14C4E__INCLUDED_)
#define AFX_APLATEDRAWABUT_H__F87B4FBD_5DA1_485A_949A_266BC4C14C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CDataManage;
class CAbut;
class CPierInfo;
class AFX_EXT_CLASS CAPlateDrawAbut
{
public:
	CAPlateDrawAbut(CDataManage *pDataMng);
	virtual ~CAPlateDrawAbut();
private:
	CDataManage *m_pDataMng;

public:
	void DrawAbutOrPier(CDomyun *pDom, long nJijum);
	void DrawAbut(CDomyun *pDom, CPierInfo *pPier);
	void DrawPier(CDomyun *pDom, CPierInfo *pPier);
};

#endif // !defined(AFX_APLATEDRAWABUT_H__F87B4FBD_5DA1_485A_949A_266BC4C14C4E__INCLUDED_)
