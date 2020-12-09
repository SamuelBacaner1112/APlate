// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// AddRate.h: interface for the CAddRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDRATE_H__D0A5DCE6_36B7_4710_857A_29E3DD4C78AC__INCLUDED_)
#define AFX_ADDRATE_H__D0A5DCE6_36B7_4710_857A_29E3DD4C78AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CAddRate : public CObject
{
public:
	CAddRate();
	CAddRate(const CAddRate& obj);
	virtual ~CAddRate();

	
public:
	double	GetOneAddRate(double dDia, BOOL bMainBar);
	double	GetAddRateFromMainBar(BOOL bMainBar);
	double	GetAddRateFromDia(double dDia);
	void	SetDataDefault();
	void	Serialize(CArchive& ar);

public:
	CAddRate& operator=(const CAddRate& obj);

	int m_nType;			// 할증률 타입 0 : 지름으로 설장, 1: 주철근으로 설정
	double m_dAddRateDia[2];// 할증률(지름 타입에 따라 다름)
	double m_dAddRateMain[2];// 할증률(주철근 타입에 따라 다름)
	double m_dBoundaryDia;  // 경계 직경--> 지름으로 설정시 사용
	double m_dFy;			// 철근의 항복강도 

};

#endif // !defined(AFX_ADDRATE_H__D0A5DCE6_36B7_4710_857A_29E3DD4C78AC__INCLUDED_)
