// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelConcData.h: interface for the CSteelConcData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELCONCDATA_H__D1BCEE63_5036_497A_87F2_55AE3078A417__INCLUDED_)
#define AFX_STEELCONCDATA_H__D1BCEE63_5036_497A_87F2_55AE3078A417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelConcData : public CObject  
{
public:
	CSteelConcData();
	virtual ~CSteelConcData();

// Attributes	
public:
	CString m_strSymbol;		//	강재 기호
	double m_BasicStrength;	//	설계기준 강도
	double	m_SafetyRate;		//	안전율
	double	m_Alpha;			//	선팽창 계수
	double	m_ElasticModulus;	//	탄성 계수
	double	m_UnitWeight;		//	단위 중량
	double	m_Temperature;		//	온도
	double	m_Epsilon;			//	건조수축에에 의한 최종 수축율
	double	m_Creep1;			//	건조수축에에 의한 크리이프계수1
	double	m_Creep2;			//	건조수축에에 의한 크리이프계수2
	double	m_BendingStress;	//	허용 휨 압축응력
	double	m_ShearStress;		//	허용 전단응력

// Implementation
public:
	void Serialize(CArchive& ar);
};

#endif // !defined(AFX_STEELCONCDATA_H__D1BCEE63_5036_497A_87F2_55AE3078A417__INCLUDED_)
