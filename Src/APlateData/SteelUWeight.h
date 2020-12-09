// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelUWeight.h: interface for the CSteelUWeight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELUWEIGHT_H__CBAD85EA_BE06_4845_B64A_F25E4BD2E0E1__INCLUDED_)
#define AFX_STEELUWEIGHT_H__CBAD85EA_BE06_4845_B64A_F25E4BD2E0E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelUWeight : public CObject  
{
public:
	CSteelUWeight();
	virtual ~CSteelUWeight();

public:
	double m_dUWeightGangjae;			///< 주철 단위중량
	double m_dUWeightAluminum;			///< 알미늄 단위중량
	double m_dUWeightChulgunCon;		///< 철근 콘크리트 단위중량
	double m_dUWeightMugunCon;			///< 무근 콘크리트 단위중량
	double m_dUWeightCement;			///< 시멘트 모르테 단위중량
	double m_dUWeightMokjae;			///< 목재 단위중량
	double m_dUWeightYukchung;			///< 역청재(방수용) 단위중량
	double m_dUWeightAsphalt;			///< 아스팔트 포장 단위중량
	double m_dUWeighGravel;				///< 자갈,쇄석 단위중량
	long	m_nTypePave;
	CString GetTypePave();
	long	GetTypePave(CString strTypePave);
	double	GetPaveUnitWeight(CString strTypePave);
public:
	void Serialize(CArchive& ar);
	void DataInit();
};

#endif // !defined(AFX_STEELUWEIGHT_H__CBAD85EA_BE06_4845_B64A_F25E4BD2E0E1__INCLUDED_)
