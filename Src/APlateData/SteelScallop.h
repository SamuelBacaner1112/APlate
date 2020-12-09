// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelScallop.h: interface for the CSteelScallop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELSCALLOP_H__360022B6_9A0B_4D0F_833E_0BF76F3C455D__INCLUDED_)
#define AFX_STEELSCALLOP_H__360022B6_9A0B_4D0F_833E_0BF76F3C455D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelScallopData : public CObject
{
public:
	CSteelScallopData();
	virtual ~CSteelScallopData();

// Attributes
public:
	CString	m_strType1;		//	단면 형식 1
	CString	m_strName1;		//	부재명	1
	CString	m_strType2;		//	단면 형식 2
	CString	m_strName2;		//	부재명 2
	CString	m_strType3;		//	단면 형식 3
	CString	m_strName3;		//	부재명 3
	long	m_nCutType;		//	모따기 타입
	
// Implementation
public:
	void Serialize(CArchive& ar);
	CSteelScallopData& operator=(const CSteelScallopData& obj);
};


class AFX_EXT_CLASS CSteelScallop  
{
public:
	CSteelScallop();
	virtual ~CSteelScallop();

// Attributes
public:
	CTypedPtrArray < CObArray, CSteelScallopData* > m_SectionArr;

// Implementation
public:
	void RemoveAll();
	void Serialize(CArchive& ar);
	void DataInit();
	long GetItemValue(CString Text);

	CString GetCutTypeByIndex(long nIdx);	
	CSteelScallopData* GetBaseScallopByIndex(long nIndex);
};

#endif // !defined(AFX_STEELSCALLOP_H__360022B6_9A0B_4D0F_833E_0BF76F3C455D__INCLUDED_)
