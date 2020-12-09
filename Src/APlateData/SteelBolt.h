// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelBolt.h: interface for the CSteelBolt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELBOLT_H__643279F7_DEE5_478E_871D_6451AA595620__INCLUDED_)
#define AFX_STEELBOLT_H__643279F7_DEE5_478E_871D_6451AA595620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum BOLT_DATA_ID {
	BDIB_STANDARD,
	BDIB_GRADE,
	BDIB_SIZE,
	BDIB_SYMBOL,
	BDIB_FORCEALLOW,
	BDIB_CENTERSPACEMIN,
	BDIB_CENTERSPACEMAX,
	BDIB_SHEARFACEDISTANCEMIN,
	BDIB_STRESSFACEDISTANCEMIN,
	BDIB_HOLEMARGIN,
	BDIB_BASEWEIGHT,
	BDIB_LENGTHPERWEIGHT,
	BDIB_NUTWEIGHT,
	BDIB_WASHERWEIGHT,
	BDIB_LENGTH,
};

class CDataManage;
class AFX_EXT_CLASS CSteelBoltData : public CObject
{
public:
	CSteelBoltData();
	virtual ~CSteelBoltData();

// Attributes
public:
	CString	m_strStandard;			// 규격
	CString	m_strGrade;				// 볼트등급
	CString	m_strSize;				// 나사호칭
	double	m_ForceAllow;			// 허용력 볼트마찰면

	double	m_CenterSpaceMin;		// 최소간격
	double	m_CenterSpaceMax;		// 최대간격
	double	m_ShearFaceDistanceMin;	// 전단면 최소 연단거리
	double	m_StressFaceDistanceMin;// 압연면 최소 연단거리
	double	m_HoleMargin;			// 볼트홀 여유치

	double	m_BaseWeight;			// 기본 중량
	double	m_LengthPerWeight;		// 길이당 질량
	double	m_NutWeight;			// 너트 중량
	double	m_WasherWeight;			// 와셔 중량
	double	m_Length;				// 체결두께+추가길이
	double	GetBoltSize();

// Implementation
public:
	void Serialize(CArchive& ar);	
};


class AFX_EXT_CLASS CSteelBolt  
{
public:
	CSteelBolt(CDataManage *pDataManage);
	virtual ~CSteelBolt();

	// Attributes	
public:
	CTypedPtrArray < CObArray, CSteelBoltData* > m_BoltArr;
	CDataManage *m_pDataManage;
	long m_nLengthCalcType;	// 0:기존방식(라운드업), 1:2사3입(강.편2-103)

// Implementation
public:
	void	Serialize(CArchive& ar);	
	void	SetDataInit();	
	double	GetBoltLength(CPlateBasicIndex *pBx, long nMaterial, CString strSize, double dThick);
	CSteelBoltData* GetBolt(CString strMaterial, double dDia);
	CSteelBoltData* GetBolt(CString strMaterial, CString strSize);

//	BOOL	Add(CBaseBolt* pBaseBolt);
	void	RemoveAll();
	CString GetStringData(long idx, BOLT_DATA_ID ID);
	long	GetIndex(CString &strSymbol);
	long	GetSize()	{ return m_BoltArr.GetSize();	}
	double	GetCalcEdgeDistance(long nBoltIdx);

	CString	GetStandard					(long idx) { return m_BoltArr[idx]->m_strStandard;				}
	CString	GetGrade					(long idx) { return m_BoltArr[idx]->m_strGrade;					}
	CString	GetSize						(long idx) { return m_BoltArr[idx]->m_strSize;					}
	double	GetForceAllow				(long idx) { return m_BoltArr[idx]->m_ForceAllow;				}
	double	GetCenterSpaceMin			(long idx) { return m_BoltArr[idx]->m_CenterSpaceMin;			}
	double	GetCenterSpaceMax			(long idx) { return m_BoltArr[idx]->m_CenterSpaceMax;			}
	double	GetShearFaceDistanceMin		(long idx) { return m_BoltArr[idx]->m_ShearFaceDistanceMin;		}
	double	GetStressFaceDistanceMin	(long idx) { return m_BoltArr[idx]->m_StressFaceDistanceMin;	}
	double	GetHoleMargin				(long idx) { return m_BoltArr[idx]->m_HoleMargin;				}
	double	GetBaseWeight				(long idx) { return m_BoltArr[idx]->m_BaseWeight;				}
	double	GetLengthPerWeight			(long idx) { return m_BoltArr[idx]->m_LengthPerWeight;			}
	double	GetNutWeight				(long idx) { return m_BoltArr[idx]->m_NutWeight;				}
	double	GetWasherWeight				(long idx) { return m_BoltArr[idx]->m_WasherWeight;				}
	double	GetLength					(long idx) { return m_BoltArr[idx]->m_Length;					}

	void	SetStandard					(long idx, CString strStandard);
	void	SetGrade					(long idx, CString strGrade);
	void	SetSize						(long idx, CString strSize);
	void	SetForceAllow				(long idx, double ForceAllow);	
	void	SetCenterSpaceMin			(long idx, double CenterSpaceMin);
	void	SetCenterSpaceMax			(long idx, double CenterSpaceMax);
	void	SetShearFaceDistanceMin		(long idx, double ShearFaceDistanceMin);
	void	SetStressFaceDistanceMin	(long idx, double StressFaceDistanceMin);
	void	SetHoleMargin				(long idx, double HoleMargin);	

	void	SetBaseWeight				(long idx, double BaseWeight);
	void	SetLengthPerWeight			(long idx, double LengthPerWeight);
	void	SetNutWeight				(long idx, double NutWeight);
	void	SetWasherWeight				(long idx, double WasherWeight);
	void	SetLength					(long idx, double Length);
};

inline	void	CSteelBolt::SetStandard					(long idx, CString strStandard)
		{  m_BoltArr[idx]->m_strStandard				=	strStandard;				}

inline	void	CSteelBolt::SetGrade					(long idx, CString strGrade)
		{  m_BoltArr[idx]->m_strGrade					=	strGrade;					}

inline	void	CSteelBolt::SetSize						(long idx, CString strSize)
		{  m_BoltArr[idx]->m_strSize					=	strSize;					}

inline	void	CSteelBolt::SetForceAllow				(long idx, double ForceAllow)
		{  m_BoltArr[idx]->m_ForceAllow					=	ForceAllow;					}

inline	void	CSteelBolt::SetCenterSpaceMin			(long idx, double CenterSpaceMin)
		{  m_BoltArr[idx]->m_CenterSpaceMin				= 	CenterSpaceMin;				}

inline	void	CSteelBolt::SetCenterSpaceMax			(long idx, double CenterSpaceMax)
		{  m_BoltArr[idx]->m_CenterSpaceMax				= 	CenterSpaceMax;				}

inline	void	CSteelBolt::SetShearFaceDistanceMin		(long idx, double ShearFaceDistanceMin)
		{  m_BoltArr[idx]->m_ShearFaceDistanceMin		= 	ShearFaceDistanceMin;		}

inline	void	CSteelBolt::SetStressFaceDistanceMin	(long idx, double StressFaceDistanceMin)
		{  m_BoltArr[idx]->m_StressFaceDistanceMin		= 	StressFaceDistanceMin;		}

inline	void	CSteelBolt::SetHoleMargin				(long idx, double HoleMargin)
		{  m_BoltArr[idx]->m_HoleMargin					= 	HoleMargin;					}

inline	void	CSteelBolt::SetBaseWeight				(long idx, double BaseWeight)
		{  m_BoltArr[idx]->m_BaseWeight					= 	BaseWeight;					}

inline	void	CSteelBolt::SetLengthPerWeight			(long idx, double LengthPerWeight)
		{  m_BoltArr[idx]->m_LengthPerWeight			= 	LengthPerWeight;			}

inline	void	CSteelBolt::SetNutWeight				(long idx, double NutWeight)
		{  m_BoltArr[idx]->m_NutWeight					= 	NutWeight;					}

inline	void	CSteelBolt::SetWasherWeight				(long idx, double WasherWeight)
		{  m_BoltArr[idx]->m_WasherWeight				= 	WasherWeight;				}

inline	void	CSteelBolt::SetLength					(long idx, double Length)
		{  m_BoltArr[idx]->m_Length						= 	Length;						}

#endif // !defined(AFX_STEELBOLT_H__643279F7_DEE5_478E_871D_6451AA595620__INCLUDED_)
