// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelStud.h: interface for the CSteelStud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELSTUD_H__019449E6_0F37_4DCC_ABC7_D14A0D3568AE__INCLUDED_)
#define AFX_STEELSTUD_H__019449E6_0F37_4DCC_ABC7_D14A0D3568AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelStudData : public CObject
{
public:
	CSteelStudData();
	virtual ~CSteelStudData();

// Attributes
public:
	CString m_strName;		// ȣĪ
	double	m_dUWeight19;
	double	m_dUWeight22;
	double	m_dUWeight25;

// Implementation
public:
	void Serialize(CArchive& ar);
	double GetStudUnitWeight(double dSize);
};

class AFX_EXT_CLASS CSteelStud  
{
public:
	CSteelStud();
	virtual ~CSteelStud();

	void RemoveAll();
	void Serialize(CArchive& ar);
	void SetDataInit();
	double GetStudUnitWeight(CString strName, double dSize);

	CSteelStudData* GetStudData(CString strName);

public:
	CTypedPtrArray < CObArray, CSteelStudData* > m_StudArr;
};

#endif // !defined(AFX_STEELSTUD_H__019449E6_0F37_4DCC_ABC7_D14A0D3568AE__INCLUDED_)
