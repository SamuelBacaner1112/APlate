// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawStandard.h: interface for the CAPlateDrawStandard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWSTANDARD_H__192F89B8_5EC7_4A0F_9839_0E6C22DC918E__INCLUDED_)
#define AFX_APLATEDRAWSTANDARD_H__192F89B8_5EC7_4A0F_9839_0E6C22DC918E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const __int64 DIM_BASE_WELDSTUD		   	= 0x0000000000000001;	// 전단연결재	
const __int64 DIM_BASE_VSTIFF			= 0x0000000000000002;	// 수직보강재	
const __int64 DIM_BASE_DIAP				= 0x0000000000000004;	// 가로보		
const __int64 DIM_BASE_SPLICE			= 0x0000000000000008;	// 현장이음		
const __int64 DIM_BASE_FACTUPPER		= 0x0000000000000010;	// 상판			
const __int64 DIM_BASE_FACTLOWER		= 0x0000000000000020;	// 하판			
const __int64 DIM_BASE_FACTWEB			= 0x0000000000000040;	// 복부판		
const __int64 DIM_BASE_JOINUPPER		= 0x0000000000000080;	// 상판&현장이음
const __int64 DIM_BASE_JOINLOWER		= 0x0000000000000100;	// 하판&현장이음
const __int64 DIM_BASE_JOINWEB			= 0x0000000000000200;	// 복부판&현장이음 
const __int64 DIM_BASE_BYUN				= 0x0000000000000400;	// 변단면
const __int64 DIM_BASE_TENDON			= 0x0000000000000800;	// 텐던위치
const __int64 DIM_BASE_SPAN				= 0x0000000000001000;	// 지간구성		
const __int64 DIM_BASE_TOTAL			= 0x0000000000002000;	// 총길이		

#define DIM_BASEHDAN_CLROAD			0x000001
#define DIM_BASEHDAN_GIRDER			0x000002
#define DIM_BASEHDAN_NODE			0x000004
#define DIM_BASEHDAN_TOTAL			0x000008

class CDataManage;
class AFX_EXT_CLASS CAPlateDrawStandard  
{
public:
	CAPlateDrawStandard(CDataManage *pDataMng);
	virtual ~CAPlateDrawStandard();

private:
	CDataManage *m_pDataMng;  

public:
	enum enum_SIDE { 
		SIDE_LEFTTOP = 0x01, SIDE_RIGHTTOP = 0x02, SIDE_LEFTBOTTOM = 0x04, SIDE_RIGHTBOTTOM = 0x08, 
		SIDE_LEFT = 0x01 | 0x04, SIDE_RIGHT = 0x02 | 0x08, SIDE_TOP = 0x01 | 0x02, 
		SIDE_BOTTOM = 0x04 | 0x08, SIDE_ALL = 0xff};

	void AddTitleOnDom(CDomyun *pDomP, double Scale, double dx, double dy, CString sTitle, BOOL bDrawScale=TRUE, int nQtyLine = 2, BOOL bLeftRotate = FALSE);
	void AddTitleOnDom(CDomyun *pDomP, double Scale, CDPoint xy, CString sTitle, BOOL bDrawScale=TRUE, int nQtyLine = 2, BOOL bLeftRotate = FALSE)
	{
		AddTitleOnDom(pDomP, Scale, xy.x, xy.y, sTitle, bDrawScale, nQtyLine, bLeftRotate);
	}
	void AddTitleOnDom(CDomyun *pDomP, double Scale, CDRect DomBound, CString sTitle, BOOL bUpperPos=TRUE, BOOL bDrawScale=TRUE, int nQtyLine=2, BOOL bLeftRotate= FALSE, CString sTitleDn="", long sTitleDnLayerType=HCAD_SUB_TITLE, double dVerSpace = 0);
};

#endif // !defined(AFX_APLATEDRAWSTANDARD_H__192F89B8_5EC7_4A0F_9839_0E6C22DC918E__INCLUDED_)
