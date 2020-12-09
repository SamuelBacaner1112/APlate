// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// NewBujeName.h: interface for the CNewBujeName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWBUJENAME_H__42C59521_49AA_11D4_8AC7_0050DA93B140__INCLUDED_)
#define AFX_NEWBUJENAME_H__42C59521_49AA_11D4_8AC7_0050DA93B140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CNewBujeNameEl
{
// Constructor
public:
	CNewBujeNameEl();
	virtual ~CNewBujeNameEl();

// Attributes
public:
	CString			m_sSecName;
	CStringArray	m_EleArr;
// Implementation
public:
	BOOL AddString(const CString& sEle);

};

class CNewBujeName
{
// Constructor
public:
	CNewBujeName();
	virtual ~CNewBujeName();

// Attributes
public:
	CPtrArray	m_NBNameArr;// CNewBujeNameEl
// Implementation
public:
	BOOL GetEleName(CStringArray& Arr,const CString& sSec) const;
	void ClearAll();
	void AddEle(const CString& sSec,const CString& sEle);

};

#endif // !defined(AFX_NEWBUJENAME_H__42C59521_49AA_11D4_8AC7_0050DA93B140__INCLUDED_)
