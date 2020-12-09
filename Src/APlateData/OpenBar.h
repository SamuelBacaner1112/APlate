// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OpenBar.h: interface for the COpenBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENBAR_H__BDA6AEE7_4984_4119_AD9B_1194B3479AB3__INCLUDED_)
#define AFX_OPENBAR_H__BDA6AEE7_4984_4119_AD9B_1194B3479AB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS COpenBar : public CObject
{
protected:
	CDPoint m_ptCen;		//개구부 중심위치 
	int		m_nTypeOpen;	//개구부 타입(0 : 원형, 1:박스형)
	double	m_dRadiusOpen;	//원형타입일때 개구부 반지름 
	double	m_dWidthOpen;	//박스타입일때 개구부 가로길이
	double	m_dLengthOpen;	//박스타입일때 개구부 세로길이 
	CString m_strOpenType;	//개구부 종류 이름 

	int		m_nCntRebar;	//한 방향의 철근 개수
	double	m_dSpaceRebar;	//철근간격 
	double	m_dDiaRebar;	//철근직경
	double	m_dLengthRebar;	//철근전체 길이
	double  m_dFy;			//철근항복강도 

public:
	COpenBar();
	COpenBar(const COpenBar& obj);
	virtual ~COpenBar();
	void Serialize(CArchive& ar);

//Method
public:
	void	SetOpenCenter(double x, double y){m_ptCen.x = x; m_ptCen.y =y; }
	CDPoint GetOpenCenter(){return m_ptCen;}
	void	SetCntRebar(int nCntRebar){m_nCntRebar = nCntRebar;}
	int		GetCntRebar(){return m_nCntRebar;}
	void	SetSpaceRebar(double dSpace){m_dSpaceRebar = dSpace;}
	double	GetSpaceRebar(){return m_dSpaceRebar;}
	void	SetDiaRebar(double dDia){m_dDiaRebar = dDia;}
	double	GetDiaRebar(){return m_dDiaRebar;}
	void	SetLengthRebar(double dLength){m_dLengthRebar = dLength;}
	double	GetLengthRebar(){return m_dLengthRebar;}
	void	SetLengthOpen(double dLength){m_dLengthOpen = dLength;}
	double	GetLengthOpen(){return m_dLengthOpen;}
	void	SetWidthOpen(double dWidth){m_dWidthOpen = dWidth;}
	double	GetWidthOpen(){return m_dWidthOpen;}
	void	SetOpenType(int nType){m_nTypeOpen = nType;}
	int		GetOpenType(){return m_nTypeOpen;}
	void	SetNameOpenType(CString strName){m_strOpenType = strName;}
	CString	GetNameOpenType(){return m_strOpenType;}
	void	SetOpenRadius(double dRadius){m_dRadiusOpen = dRadius;}
	double	GetOpenRadius(){return m_dRadiusOpen;}
	void	SetFy(double dFy){m_dFy = dFy;}
	double	GetFy(){return m_dFy;}

//Draw Method
public:
	CString GetTextDia();
	void DrawOpen(CDomyun* pDom);

};

#endif // !defined(AFX_OPENBAR_H__BDA6AEE7_4984_4119_AD9B_1194B3479AB3__INCLUDED_)
