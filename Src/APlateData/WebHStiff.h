// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WebHStiff.h: interface for the CWebHStiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBHSTIFF_H__72B7C7B8_8176_45E6_AA29_73249D135334__INCLUDED_)
#define AFX_WEBHSTIFF_H__72B7C7B8_8176_45E6_AA29_73249D135334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const long WEBCOUNT = 2;
const long WEBHSTIFFSUMAX = 6;

#define WEB_LEFT 0
#define WEB_RIGHT 1
#define POS_UP 0
#define POS_DN 1

#define WEB_SIDE_BOTH	0
#define WEB_SIDE_LEFT	1
#define WEB_SIDE_RIGHT	2
#define WEB_SIDE_AUTO	3

class AFX_EXT_CLASS CWebHStiff    : public CPlateBxObject
{
// Constructor
public:
	CWebHStiff();
	virtual ~CWebHStiff();

// Attributes
public:		
	long	m_nDanSu[2][WEBCOUNT];
	double	m_dStationStt[WEBCOUNT][WEBHSTIFFSUMAX];		//좌측(시작) 스테이션
	double	m_dStationEnd[WEBCOUNT][WEBHSTIFFSUMAX];		//우측(종점) 스테이션
	double	m_dHeightFactor[WEBCOUNT][WEBHSTIFFSUMAX];	//높이 비율(상판 하단 기준 - 헌치부 포함 => +200)
	double	m_dLengthHStiff[WEBCOUNT][WEBHSTIFFSUMAX];	//실제 길이
	double	m_dDistStt[WEBCOUNT][WEBHSTIFFSUMAX];			//좌측 꺽임 길이(잭업보강재 사이에 있을 경우 등)
	double	m_dDistEnd[WEBCOUNT][WEBHSTIFFSUMAX];			//우측 꺽임 길이(잭업보강재 사이에 있을 경우 등)
	double	m_dThickHStiff[WEBCOUNT];		//수평보강재 두께
	double	m_dWidthHStiff[WEBCOUNT];		//수평보강재 너비
	long	m_nPos;							// 0:양측 1:좌측 2:우측

// Implementation
public:
	void	SetFillAssmVariable(CDoubleArray& dArr, BOOL bLeft);
	virtual DWORD GetLinkType();
	void	Clear();
	void	SetTotalDansu(BOOL bUpper, BOOL bLeft, long nDan);
	long	GetTotalDansu(BOOL bUpper, BOOL bLeft) const;
	BOOL	IsEmpty(long nDan, BOOL bLeft) const;
	double	GetLength(long nDan, BOOL bLeft, DIMTYPE nDimType) const;
	double	GetHeightHStiffBySta(long nDan, BOOL bLeft, double sta) const;
	double	GetHeightHStiffOnStt(long nDan, BOOL bLeft, BOOL bStt) const;
	double	GetAngleRadianHStiff(long nRow, BOOL bLeft) const;
	long	GetUpperDan(BOOL bLeft) const;
	long	GetLowerDan(BOOL bLeft) const;
	void	Serialize(CArchive& ar);
	CWebHStiff& operator=(const CWebHStiff& obj);
};


#endif // !defined(AFX_WEBHSTIFF_H__72B7C7B8_8176_45E6_AA29_73249D135334__INCLUDED_)
