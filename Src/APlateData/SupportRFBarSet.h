// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SupportRFBarSet.h: interface for the CSupportRFBarSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPORTRFBARSET_H__799910FE_7BDE_4284_A6FF_C92D536BD9C2__INCLUDED_)
#define AFX_SUPPORTRFBARSET_H__799910FE_7BDE_4284_A6FF_C92D536BD9C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SupportRFBar.h"

class AFX_EXT_CLASS CSupportRFBarSet : public CObject 
{
private:
	vector<CSupportRFBar>	m_vecSupportRFBar;	//�������� ����ö��
	double	m_dDiaSptRFBar;					//�������� ����ö�� ����
	double	m_dLenSptRFBar;					//�������� ����ö�� ��ü���� 
	double	m_dArcRadiusSptRFBar;			//�������� ����ö�� Arc Radius
	double	m_dArcAngSptRFBar;				//�������� ����ö�� Arc Angle
	double	m_dFy;							//ö���׺����� 

public:
	CSupportRFBarSet();
	virtual ~CSupportRFBarSet();

	
public:
	int		GetSize();
	void	SetDataDefault(BOOL bRecommend);
	void	AddRFBar(CSupportRFBar rfbar);
	BOOL	RemoveRFBar(int nIdx);
	void	RemoveAll();
	void	RemoveRebarPos();
	void	Serialize(CArchive& ar);
	CString GetTextRebarDia();
	CSupportRFBar* GetAt(int nIdx);

//Draw
public:
	void	DimRebar(CDomyun* pDom);
	void	DrawStud(CDomyun* pDom);
	void	DrawRebar(CDomyun* pDom);

//Data Setting
public:
	void	SetArcAngleRFBar(double dAngle);
	double  GetArcAngleRFBar(){return m_dArcAngSptRFBar;}

	void	SetArcRadiusRFBar(double dRadius);
	double	GetArcRadiusRFBar(){return m_dArcRadiusSptRFBar;}

	void	SetLenRFBar(double dLen);
	double	GetLenRFBar(){return m_dLenSptRFBar;}

	void	SetDiaRFBar(double dDia);
	double	GetDiaRFBar(){return m_dDiaSptRFBar;}

	void	SetFy(double dFy){m_dFy = dFy;}
	double	GetFy(){return m_dFy;}
};

#endif // !defined(AFX_SUPPORTRFBARSET_H__799910FE_7BDE_4284_A6FF_C92D536BD9C2__INCLUDED_)
