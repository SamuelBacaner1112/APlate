// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelConc.h: interface for the CSteelConc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELCONC_H__ECA56F03_C3DC_4AF2_855F_391EF5561E32__INCLUDED_)
#define AFX_STEELCONC_H__ECA56F03_C3DC_4AF2_855F_391EF5561E32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelConc  
{
public:
	CSteelConc();
	virtual ~CSteelConc();

// Attributes	
protected:
	CTypedPtrArray < CObArray, CSteelConcData* > m_ConArr;

// Implementation
public:
	void	Serialize(CArchive& ar);
	void	SetDataInit();
	void	RemoveAll();
	CString GetStringData(long idx, long ID);
	long	GetIndex(CString strSymbol);
	long	GetSize()						{ return m_ConArr.GetSize();				}

	CString	GetSymbol			(long idx)	{ return m_ConArr[idx]->m_strSymbol;		}
	double GetBasicStrength	(long idx)	{ return m_ConArr[idx]->m_BasicStrength;	}
	double	GetSafetyRate		(long idx)	{ return m_ConArr[idx]->m_SafetyRate;		}
	double	GetAlpha			(long idx)	{ return m_ConArr[idx]->m_Alpha;			}
	double	GetElasticModulus	(long idx)	{ return m_ConArr[idx]->m_ElasticModulus;	}
	double	GetUnitWeight		(long idx)	{ return m_ConArr[idx]->m_UnitWeight;		}
	double	GetTemperature		(long idx)	{ return m_ConArr[idx]->m_Temperature;		}
	double	GetEpsilon			(long idx)	{ return m_ConArr[idx]->m_Epsilon;			}
	double	GetCreep1			(long idx)	{ return m_ConArr[idx]->m_Creep1;			}
	double	GetCreep2			(long idx)	{ return m_ConArr[idx]->m_Creep2;			}
	double	GetBendingStress	(long idx)	{ return m_ConArr[idx]->m_BendingStress;	}
	double	GetShearStress		(long idx)	{ return m_ConArr[idx]->m_ShearStress;		}

//	CSteelConcData GetBaseCon			(long idx)	{ return *(m_ConArr[idx]);					}

	void	SetSymbol			(long idx, CString strSymbol);
	void	SetBasicStrength	(long idx, double BasicStrength);
	void	SetSafetyRate		(long idx, double SafetyRate);
	void	SetAlpha			(long idx, double Alpha);
	void	SetElasticModulus	(long idx, double ElasticModulus);
	void	SetUnitWeight		(long idx, double UnitWeight);
	void	SetTemperature		(long idx, double Temperature);
	void	SetEpsilon			(long idx, double Epsilon);
	void	SetCreep1			(long idx, double Creep1);
	void	SetCreep2			(long idx, double Creep2);
	void	SetBendingStress	(long idx, double BendingStress);
	void	SetShearStress		(long idx, double ShearStress);

	void	SetBaseCon			(long idx, CSteelConcData* pBaseCon);
};

inline	void	CSteelConc::SetSymbol			(long idx, CString strSymbol)
		{  m_ConArr[idx]->m_strSymbol		= strSymbol;		}
inline	void	CSteelConc::SetBasicStrength	(long idx, double BasicStrength)
		{  m_ConArr[idx]->m_BasicStrength	= BasicStrength;				}
inline	void	CSteelConc::SetSafetyRate		(long idx, double SafetyRate)
		{  m_ConArr[idx]->m_SafetyRate		= SafetyRate;		}
inline	void	CSteelConc::SetAlpha			(long idx, double Alpha)
		{  m_ConArr[idx]->m_Alpha			= Alpha;			}
inline	void	CSteelConc::SetElasticModulus	(long idx, double ElasticModulus)
		{  m_ConArr[idx]->m_ElasticModulus	= ElasticModulus;	}
inline	void	CSteelConc::SetUnitWeight		(long idx, double UnitWeight)
		{  m_ConArr[idx]->m_UnitWeight		= UnitWeight;		}
inline	void	CSteelConc::SetTemperature		(long idx, double Temperature)
		{  m_ConArr[idx]->m_Temperature		= Temperature;		}
inline	void	CSteelConc::SetEpsilon			(long idx, double Epsilon)
		{  m_ConArr[idx]->m_Epsilon			= Epsilon;			}
inline	void	CSteelConc::SetCreep1			(long idx, double Creep1)
		{  m_ConArr[idx]->m_Creep1			= Creep1;			}
inline	void	CSteelConc::SetCreep2			(long idx, double Creep2)
		{  m_ConArr[idx]->m_Creep2			= Creep2;			}
inline	void	CSteelConc::SetBendingStress	(long idx, double BendingStress)
		{  m_ConArr[idx]->m_BendingStress	= BendingStress;	}
inline	void	CSteelConc::SetShearStress		(long idx, double ShearStress)
		{  m_ConArr[idx]->m_ShearStress		= ShearStress;		}

#endif // !defined(AFX_STEELCONC_H__ECA56F03_C3DC_4AF2_855F_391EF5561E32__INCLUDED_)
