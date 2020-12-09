// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// CentSeparation.h: interface for the CCentSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CENTSEPARATION_H__C0116D1E_F00B_47E8_8122_FBF4B5E6FA9B__INCLUDED_)
#define AFX_CENTSEPARATION_H__C0116D1E_F00B_47E8_8122_FBF4B5E6FA9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AREA_NUM	7

//방호벽 종류, 철도교일 경우, 다이얼로그에서 입력 받을 경우 사용
#define  HT_NONE				0		//도로
#define  HT_GUARDFENCE			1		//방호벽
#define	 HT_CENTERGUARDFENCE	2		//중분대
#define  HT_PEOPLEROAD			3		//보도
//
//#define  HT_DETECTGRAVEL	2		//자갈막이
//#define  HT_VERDOSANG		4		//직결식도상
//#define  HT_GRAVELDOSANG	5		//자갈도상
//#define  HT_GUARDFENCEANDROAD 6		//연석+보도
//#define  HT_ELECHOLE		7		//통신구
//#define  HT_ELECHOLEANDROAD 8		//통신구+보도

//방호벽 종류, CCentSeparation 객체에 세팅하여 사용
#define  SLAB_GUARD_LEFT		0		//방호벽(좌)		
#define  SLAB_GUARD_RIGH		1		//방호벽(우)		
#define  SLAB_GUARD_CENT		2		//중분대			
#define  SLAB_CURB_LEFT			3		//연석(좌)			
#define  SLAB_CURB_RIGH			4		//연석(우)			
#define  SLAB_GRAVEL_LEFT		5		//자갈막이(좌)		
#define  SLAB_GRAVEL_RIGH		6		//자갈막이(우)		
#define  SLAB_CURB_WALK_LEFT	7		//연석+보도(좌)		
#define  SLAB_CURB_WALK_RIGH	8		//연석+보도(우)		
#define  SLAB_CABLE_LEFT		9		//통신구(좌)		
#define  SLAB_CABLE_RIGH		10		//통신구(우)		
#define  SLAB_CABLE_WALK_LEFT	11		//통신구+보도(좌)	
#define  SLAB_CABLE_WALK_RIGH	12		//통신구+보도(우)	

class CGuardFenceRebar;

class AFX_EXT_CLASS CCentSeparation : public CObject
{
public:
	CCentSeparation();
	virtual ~CCentSeparation();
	enum PLACE	{ LEFT, CENTER, RIGHT } nPlace;

	long	GetPlace();
	double	GetWidth() const;
	double	GetHeight() const;
	double	GetDistCentroid();
	void	GetXyGuardFence(CDPoint A[9]) const;
	void	SetDataDefault1(CCentSeparation *pGuard, long nGuard , long bLeft);

	double	GetArea();
	void	InitCalculate(double dUnitWeight = 2.5);

public:
	
	void SetSeperationType(CString str);
	// void SetInitData();
	CString		m_strName;
	double		m_D1;
	double		m_W1;
	double		m_W2;
	double		m_W3;
	double		m_W4;
	double		m_W5;
	double		m_H1;
	double		m_H2;
	double		m_H3;
	long		m_nSepType;
	long		m_nDirType;

	double		m_DistCentroid;	
	double		m_Area[AREA_NUM];			// 최대 면적의 개수 (7가지)
	double		m_DistArea[AREA_NUM];		// 각 면적에 대한 도심까지의 거리 (기준점은 좌측 하단으로 부터의 거리)
	vector<CDPoint> m_dAreaCenter;			// 면적의 중앙점
	vector<pair<CString, CDPoint> > m_strAreaFormula;	// 면적에 대한 산식

	void Serialize( CArchive& ar );
	void SerializeOld( CArchive& ar );
	CCentSeparation& operator=(const CCentSeparation& obj);

public:
	CGuardFenceRebar* GetRebar();

private:
	CGuardFenceRebar* m_pRebar;
};

#endif // !defined(AFX_CENTSEPARATION_H__C0116D1E_F00B_47E8_8122_FBF4B5E6FA9B__INCLUDED_)
