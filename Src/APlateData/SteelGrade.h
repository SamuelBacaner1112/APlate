// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelGrade.h: interface for the CSteelGrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELGRADE_H__0951B176_892C_4313_BB01_5ADE3CAD0339__INCLUDED_)
#define AFX_STEELGRADE_H__0951B176_892C_4313_BB01_5ADE3CAD0339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelGradeData : public CObject
{
public:
	CSteelGradeData() {};

// Attributes
public:
	CString	m_strGrade;
	double	m_weDB;
	double	m_weDBPt;
	double	m_weDBPf;
	double	m_weDBPr;
	double	m_weDL;
	double	m_weDLPm;
	double	m_weDLPs;

// Implementation
public:
	void Serialize(CArchive& ar)
	{
		if(ar.IsStoring()) 
		{
			ar << m_strGrade;
			ar << m_weDB;
			ar << m_weDBPt;
			ar << m_weDBPf;
			ar << m_weDBPr;
			ar << m_weDL;
			ar << m_weDLPm;
			ar << m_weDLPs;
		}
		else 
		{
			ar >> m_strGrade;
			ar >> m_weDB;
			ar >> m_weDBPt;
			ar >> m_weDBPf;
			ar >> m_weDBPr;
			ar >> m_weDL;
			ar >> m_weDLPm;
			ar >> m_weDLPs;
		}
	}
};

class AFX_EXT_CLASS CSteelGrade  
{
public:
	CSteelGrade();
	virtual ~CSteelGrade();

// Attributes
protected:
	long	m_sidx;
	CTypedPtrArray < CObArray, CSteelGradeData* > m_GradeArr;

// Implementation
public:
	void	Serialize(CArchive& ar);
	void	SetDataInit();
	void	RemoveAll();
	CString GetStringData(long idx, long ID);
	long	GetIndex(CString strGrade);
	long	GetSize()				{ return m_GradeArr.GetSize();			}

	CString	GetGrade(long idx)		{ return m_GradeArr[idx]->m_strGrade;	}
	double	GetDB	(long idx)		{ return m_GradeArr[idx]->m_weDB;		}
	double	GetDBPt	(long idx)		{ return m_GradeArr[idx]->m_weDBPt;		}
	double	GetDBPf	(long idx)		{ return m_GradeArr[idx]->m_weDBPf;		}
	double	GetDBPr	(long idx)		{ return m_GradeArr[idx]->m_weDBPr;		}
	double	GetDL	(long idx)		{ return m_GradeArr[idx]->m_weDL;		}
	double	GetDLPm	(long idx)		{ return m_GradeArr[idx]->m_weDLPm;		}
	double	GetDLPs	(long idx)		{ return m_GradeArr[idx]->m_weDLPs;		}

	CSteelGradeData* GetBaseGrade(long idx){ return m_GradeArr[idx];				}

//	long	GetSize()				{ return m_Size;						}

	void	SetGrade(long idx, CString	strGrade);
	void	SetDB	(long idx, double	weDB);
	void	SetDBPt	(long idx, double	weDBPt);
	void	SetDBPf	(long idx, double	weDBPf);
	void	SetDBPr	(long idx, double	weDBPr);
	void	SetDL	(long idx, double	weDL);
	void	SetDLPm	(long idx, double	weDLPm);
	void	SetDLPs	(long idx, double	weDLPs);

	void	SetBaseGrade(long idx, CSteelGradeData* pBaseGrade);
};

inline	void	CSteelGrade::SetGrade	(long idx, CString	strGrade)
		{  m_GradeArr[idx]->m_strGrade	= strGrade;		}
inline	void	CSteelGrade::SetDB	(long idx, double	weDB)
		{  m_GradeArr[idx]->m_weDB		= weDB;			}
inline	void	CSteelGrade::SetDBPt	(long idx, double	weDBPt)
		{  m_GradeArr[idx]->m_weDBPt		= weDBPt;		}
inline	void	CSteelGrade::SetDBPf	(long idx, double	weDBPf)
		{  m_GradeArr[idx]->m_weDBPf		= weDBPf;		}
inline	void	CSteelGrade::SetDBPr	(long idx, double	weDBPr)
		{  m_GradeArr[idx]->m_weDBPr		= weDBPr;		}
inline	void	CSteelGrade::SetDL	(long idx, double	weDL)
		{  m_GradeArr[idx]->m_weDL		= weDL;			}
inline	void	CSteelGrade::SetDLPm	(long idx, double	weDLPm)
		{  m_GradeArr[idx]->m_weDLPm		= weDLPm;		}
inline	void	CSteelGrade::SetDLPs	(long idx, double	weDLPs)
		{  m_GradeArr[idx]->m_weDLPs		= weDLPs;		}

#endif // !defined(AFX_STEELGRADE_H__0951B176_892C_4313_BB01_5ADE3CAD0339__INCLUDED_)
