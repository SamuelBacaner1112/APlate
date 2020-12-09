// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SupportRFBar.h: interface for the CSupportRFBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPORTRFBAR_H__D9474E23_BB66_4294_AAC6_0932492D8978__INCLUDED_)
#define AFX_SUPPORTRFBAR_H__D9474E23_BB66_4294_AAC6_0932492D8978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSupportRFBar : public CObject 
{
private:
	double			m_dDia;
	double			m_dLength;
	double			m_dArcAngle;
	double			m_dArcRadius;
	CDPoint			m_ptCenter;
	CDPointArray	m_ptArrStud;

public:
	CSupportRFBar();
	CSupportRFBar(const CSupportRFBar& obj);
	virtual ~CSupportRFBar();
	CSupportRFBar& operator=(const CSupportRFBar& obj);

private:
	double	GetStudLength();

//DataSetting
public:
	void		SetDia(double dDia){m_dDia = dDia;}
	double		GetDia(){return m_dDia;}
	void		SetTotalLength(double dLength){m_dLength = dLength;}
	double		GetTotalLength(){return m_dLength;}
	void		SetArcAngle(double dAngle){m_dArcAngle = dAngle;}
	double		GetArcAngle(){return m_dArcAngle;}
	void		SetArcRadius(double dRadius){m_dArcRadius = dRadius;}
	double		GetArcRadius(){return m_dArcRadius;}
	void		SetStudCenter(CDPoint ptCenter){m_ptCenter = ptCenter;}
	CDPoint		GetStudCenter(){return m_ptCenter;}
	void		SetPosStud(CDPointArray ptArrStud);
	CDPointArray GetPosStud(double dDis = 0);
	void		Serialize(CArchive& ar);

//Method
public:
	CDPointArray	GetPosRebar(BOOL bLeft);
	CDPoint			GetCalcStudCenter();
	void			SetDataDefault(BOOL bRecommend);
	CDPoint			GetArcCenter(CDPointArray ptArrPos, BOOL bLeft);
	void			MoveOrgBarToStudCen(CDPointArray& ptArr, BOOL bLeft);
	CDPointArray	GetPosOrgRebar(BOOL bLeft);
	void			GetXyMatchStudAndRebar(CDPoint& ptUpper, CDPoint& ptLower, BOOL bLeft);
	double			GetLineLength();
	double			GetArcLength();
	CDPointArray	GetPosDetailRebar(BOOL bLeft, double dDis = 0);
	double			GetDetailRadius();
	CDPoint			GetOrgStudCenter(BOOL bLeft);

//Draw
public:
	double			GetAngleStud();
	void			RotateRebar(CDPointArray& ptArr);
	void			DrawDetailCircleText(CDomyun* pDom, CString strText);
	void			MarkStud(CDomyun* pDom, double dDis = 0);
	void			DrawDetailCircle(CDomyun* pDom);
	void			DrawDetail(CDomyun* pDom, double dDis = 0);
	void			DrawRebar(CDomyun* pDom);
	void			DrawStud(CDomyun* pDom, double dRadius = 10, double dDis = 0);
};

#endif // !defined(AFX_SUPPORTRFBAR_H__D9474E23_BB66_4294_AAC6_0932492D8978__INCLUDED_)
