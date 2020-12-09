// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BoxOpenBar.h: interface for the CBoxOpenBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOXOPENBAR_H__1AF00653_6D57_40E1_AD5D_71AA90055891__INCLUDED_)
#define AFX_BOXOPENBAR_H__1AF00653_6D57_40E1_AD5D_71AA90055891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OpenBar.h"

class AFX_EXT_CLASS CBoxOpenBar : public COpenBar  
{
private:
	double m_dCover;		//개구부 철근의 피복두께

public:
	CBoxOpenBar();
	CBoxOpenBar(const CBoxOpenBar& obj):COpenBar(obj){m_dCover = obj.m_dCover;}
	
	virtual ~CBoxOpenBar();
	void			Serialize(CArchive& ar);

//Method
public:
	void			SetCover(double dCover){m_dCover = dCover;}
	double			GetCover(){return m_dCover;}
	void			GetPosFirstDiagonalBar(CDPoint& ptStt, CDPoint& ptEnd, int nType);
	void			GetPosFirstStraightBar(CDPoint& ptStt, CDPoint& ptEnd, int nType);
	CDPointArray	GetPosStraightBar(int nType);
	CDPointArray	GetPosDiagonalBar(int nType);
	void			SetDataDefault(BOOL bRecommend);
};

#endif // !defined(AFX_BOXOPENBAR_H__1AF00653_6D57_40E1_AD5D_71AA90055891__INCLUDED_)
