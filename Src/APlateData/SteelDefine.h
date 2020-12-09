// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelDefine.h: interface for the CSteelDefine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELDEFINE_H__B9965747_77E0_4B4E_84BB_9AC7F72D6D67__INCLUDED_)
#define AFX_STEELDEFINE_H__B9965747_77E0_4B4E_84BB_9AC7F72D6D67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum	STLDEF_DATA_ID
{
	BDISD_CLASS,
	BDISD_VARNAME,
	BDISD_SYMBOL,
};


class AFX_EXT_CLASS	CSteelDefineData	:	public CObject
{
// Constructor
public:
	CSteelDefineData() {};

// Attributes
public:
	CString	m_strClass;		// 대분류
	CString m_strVarName;	// 권장 변수명
	CString m_strSymbol;	// Default Symbol

// Implementation
public:
	void Serialize(CArchive& ar)
	{
		if(ar.IsStoring()) 
		{
			ar << m_strClass;			
			ar << m_strSymbol;
		}
		else 
		{
			ar >> m_strClass;
			ar >> m_strSymbol;
		}
	}
};

class AFX_EXT_CLASS CSteelDefine
{
public:
	CSteelDefine();
	virtual ~CSteelDefine();

// Attributes	
public:
	CTypedPtrArray < CObArray, CSteelDefineData* > m_SteelDefArr;

// Implementation
public:
	void	Serialize(CArchive& ar);
	void	SetDataInit();
	void	RemoveAll();
	CString GetStringData(long idx, STLDEF_DATA_ID ID);
	long	GetSize()	{ return m_SteelDefArr.GetSize();	}

	CString	GetClass  (long idx)	{ return	m_SteelDefArr[idx]->m_strClass;	}
	CString	GetVarName(long idx)	{ return	m_SteelDefArr[idx]->m_strVarName;}
	CString	GetSymbol (long idx)	{ return	m_SteelDefArr[idx]->m_strSymbol;}

	void	SetClass  (long idx, CString strClass );
	void	SetVarName(long idx, CString strVarName);
	void	SetSymbol (long idx, CString strSymbol);

	void	SetBaseSteelDef (long idx, CSteelDefineData* pBaseSteelDef);
};

#endif // !defined(AFX_STEELDEFINE_H__B9965747_77E0_4B4E_84BB_9AC7F72D6D67__INCLUDED_)
