// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelRange.h: interface for the CSteelRange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELRANGE_H__F0F16DA6_557C_4A0D_962C_83B9A3EA63F8__INCLUDED_)
#define AFX_STEELRANGE_H__F0F16DA6_557C_4A0D_962C_83B9A3EA63F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum	STLRANGE_DATA_ID
{
	BDISR_VARNAME	= 0x01,
	BDISR_STANDARD	= 0x02,
	BDISR_SYMBOL	= 0x04,
};

class AFX_EXT_CLASS CSteelRangeData : public CObject
{
public:
	CSteelRangeData() {};

public:
	CString	m_strVarName;	
	CString	m_strStandard;
	CString	m_strSymbol;

// Implementation
public:
	void Serialize(CArchive& ar)
	{
		if(ar.IsStoring()) 
		{
			ar << m_strVarName;
			ar << m_strStandard;
			ar << m_strSymbol;
		}
		else 
		{
			CString strTemp;

			ar >> m_strVarName;
			ar >> strTemp;
			
			strTemp.Replace(" ", "");
			m_strStandard = strTemp;

			ar >> m_strSymbol;

			//DB적용 전 예외처리.
			if(m_strSymbol.Find("SWS400") > -1)
			{
				m_strSymbol =	"SM400";
				m_strSymbol +=	"B";
			}
			
			else if(m_strSymbol.Find("SWS490") > -1)
			{
				m_strSymbol	=	"SM490";
				m_strSymbol +=	"B";
			}
			else if(m_strSymbol.Find("SWS490Y") > -1 || m_strSymbol.Find("SWS520") > -1)
			{
				m_strSymbol =	"SMA490";
				m_strSymbol +=	"B";
			}
			else if(m_strSymbol.Find("SWS570") > -1)
			{
				m_strSymbol =	"SMA570";
				m_strSymbol	+=	"B";
			}
			
			if(m_strSymbol == "SM400"  || m_strSymbol == "SM490"	||
				m_strSymbol == "SM490Y" || m_strSymbol == "SMA400"	||
				m_strSymbol == "SMA490" || m_strSymbol == "SMA570")	
			{
				m_strSymbol += "B";
			}
		}
	}
};
class CDataManage;
class AFX_EXT_CLASS CSteelRange  
{
public:
	CSteelRange(CDataManage	*pManage = NULL);
	virtual ~CSteelRange();

// Attributes	
protected:
	CSteelRangeData	m_KeyRec;
	long			m_Key;		//	Search Key;(검색 조건)
	long			m_idx;		//	Current Search pos
	CTypedPtrArray < CObArray, CSteelRangeData* > m_SteelRangeArr;
	CDataManage		*m_pDataManage;
// Implementation
public:
	void	Serialize(CArchive& ar);
	void	SetDataInit();

	void	ClearKey();
	void	AllocKey(CSteelRangeData* pse, long KeyMode);
	BOOL	StartKey();
	BOOL	NextKey();
	CSteelRangeData*	GetRecord(CSteelRangeData* psr);

	void	RemoveAll();
	CString GetStringData(long idx, STLRANGE_DATA_ID ID);
	long	GetSize()	{ return m_SteelRangeArr.GetSize();	}

	CString	GetVarName	(long idx)	{ return m_SteelRangeArr[idx]->m_strVarName;	}
	CString	GetStandard	(long idx)	{ return m_SteelRangeArr[idx]->m_strStandard;	}
	CString	GetSymbol	(long idx)	{ return m_SteelRangeArr[idx]->m_strSymbol;		}

//	CSteelRangeData	GetBaseSteelRange(long idx) { return *(m_SteelRangeArr[idx]);	}

	void	SetVarName	(long idx, CString strVarName	);
	void	SetStandard	(long idx, CString strStandard	);
	void	SetSymbol	(long idx, CString strSymbol	);

	void	SetBaseSteelRange	(long idx, CSteelRangeData* pBaseSteelRange);
};

inline	void	CSteelRange::SetVarName	(long idx, CString strVarName	)
		{ 	m_SteelRangeArr[idx]->m_strVarName	=	strVarName;	}
inline	void	CSteelRange::SetStandard	(long idx, CString strStandard	)
		{ 	m_SteelRangeArr[idx]->m_strStandard	=	strStandard;	}
inline	void	CSteelRange::SetSymbol	(long idx, CString strSymbol	)
		{ 	m_SteelRangeArr[idx]->m_strSymbol	=	strSymbol;	}

#endif // !defined(AFX_STEELRANGE_H__F0F16DA6_557C_4A0D_962C_83B9A3EA63F8__INCLUDED_)
