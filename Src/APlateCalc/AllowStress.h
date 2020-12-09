// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// AllowStress.h: interface for the CAllowStress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLOWSTRESS_H__84F60A24_DA8D_4293_8DD4_CA070A37B9DC__INCLUDED_)
#define AFX_ALLOWSTRESS_H__84F60A24_DA8D_4293_8DD4_CA070A37B9DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutCalcStd;
#define ALLOW_QTY_MAX					3		// 두께 구분

#define ALLOW_SHEARSTRESS				0		// 허용전단응력
#define ALLOW_TENSIONSTRESS				1		// 허용인장응력
#define ALLOW_COMPSTRESS				2		// 허용압축응력
#define ALLOW_BUCKLINGCOEFF				3		// 자유돌출판의 국부좌굴에 대한 허용응력 계수
#define ALLOW_BUCKLING					4		// 자유돌출판의 국부좌굴에 대한 허용응력
#define ALLOW_STDYIELDPOINT_UP			5		// 판두께에 따른 기준항복점 - 상판
#define ALLOW_STDYIELDPOINT_LO			6		// 판두께에 따른 기준항복점 - 하판
#define ALLOW_BENDINGCOMP_CASE1			7		// 허용 휨압축응력 - 압축플랜지가 콘크리트바닥판 등에 직접고정되어 있는경우 횡좌굴이 일어나기 어려운경우(Box 형)
#define ALLOW_BENDINGCOMP_CASE2			8		// 허용 휨압축응력 - 상기 그 외의 경우(판형)
#define ALLOW_BUCKLING_UNCONSIDER		9		// 국부좌굴을 고려하지 않은 허용축방향 압축응력
#define ALLOW_FLANGE_SPACE				10		// 수직보강재를 생략할 수 있는 플랜지 순간격의 최대치
#define ALLOW_WEBMINTHICK				11		// 복부판 최소두께
#define ALLOW_STDYIELDPOINT_WEB			12		// 판두께에 따른 기준항복점 - 복부판

#define THICK40BELOW					0		// 두께 40이하
#define THICK75BELOW					1		// 두께 40초과 75이하
#define THICK100BELOW					2		// 두께 75초과 100이하

typedef	struct AFX_EXT_CLASS _ALLOWSTRESS
{
	CString	m_strMaterial;
	double	m_dAllowStress;
	double	m_dCoeff1;
	double	m_dCoeff2;
	double	m_dCoeff3;
	double	m_dCoeff4;
	double	m_dCoeff5;
} ALLOWSTRESS;

class AFX_EXT_CLASS CAllowStress  
{
public:
	CAllowStress(long nTypeAllow, long nMaterial);
	virtual ~CAllowStress();

	void InitData();
	double GetAllowStress(CString strMaterial, double dThick);
	long GetTypeAllow()				{ return m_nTypeAllow;		};
	long GetIdxByThick(double dThick);
	long GetSteelSize()				{ return m_nSize;			};
	long GetThickSize()				{ return m_nThickSize;		};
	double GetDistFlangeFixed()		{ return m_dDistFlangeFixed;};
	double GetWidthFlange()			{ return m_dWidthFlange;	};
	double GetAreaWeb()				{ return m_dAreaWeb;		};
	double GetAreaConc()			{ return m_dAreaConc;		};
	long GetMaterialIndex(CString strMaterial);
	CString GetAllowStressTitle();
	long GetDecimalCiphers();

	void InsertItem(long nThickIdx, CString strMaterial, double dAllowStress, double dAllowCoeff1=0.0, double dAllowCoeff2=0.0, double dAllowCoeff3=0.0, double dAllowCoeff4=0.0, double dAllowCoeff5=0.0);
	void SetAllowBendingStress(double dAw, double dAc, double dDistFlangeFixed, double dWidthFlange);
	void SetAllowBucklingUnconsider(double dLengthBuckling, double dTurnigRadius);
	void SetAllowBuckling(double dThick, double dWidth);
	vector<ALLOWSTRESS>	m_vtAllowStress[ALLOW_QTY_MAX];

private:
	long	m_nTypeAllow;
	long	m_nMaterial;
	long	m_nSize;
	long	m_nThickSize;
	CString	m_strMaterial;
	double	m_dAreaWeb;					// 복부판의 총단면적(mm2)
	double	m_dAreaConc;				// 압축플랜지 총단면적(mm2)
	double	m_dDistFlangeFixed;			// 압축플랜지 고정점간이 거리(mm)
	double	m_dWidthFlange;				// 압축플랜지 폭(mm)
	double	m_dK;						// sqrt(3+Aw/2Ac)
	double	m_dLengthBuckling;			// 부재의 유효좌굴길이
	double	m_dTurningRadius;			// 부재 총단면의 단면회전반경
	double	m_dThick;					// Material 두께
	double	m_dWidth;					// Material 폭

	void ClearStress();
};

#endif // !defined(AFX_ALLOWSTRESS_H__84F60A24_DA8D_4293_8DD4_CA070A37B9DC__INCLUDED_)
