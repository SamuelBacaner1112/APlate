// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelWeld.h: interface for the CSteelWeld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELWELD_H__646D499E_1FCD_445E_9372_6CBFB4D5D867__INCLUDED_)
#define AFX_STEELWELD_H__646D499E_1FCD_445E_9372_6CBFB4D5D867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelWeldData : public CObject
{
public:
	CSteelWeldData();
	virtual ~CSteelWeldData();

// Attributes
public:
	CString	m_strType1;		//	�ܸ� ���� 1
	CString	m_strName1;		//	�����	1
	CString	m_strType2;		//	�ܸ� ���� 2
	CString	m_strName2;		//	����� 2
	long	m_nWeldType;	//	��������
	long	m_nImproveType;	//	��������
	long	m_nLayType;		//	�������

// Implementation
public:
	void Serialize(CArchive& ar);
	CSteelWeldData& operator=(const CSteelWeldData& obj);
};

class AFX_EXT_CLASS CSteelWeld  
{
public:
	CSteelWeld();
	virtual ~CSteelWeld();

// Attributes
public:
	CTypedPtrArray < CObArray, CSteelWeldData* > m_SectionArr;

// Implementation
public:
	CSteelWeldData* GetBaseWeldByIndex(long nIndex);
	void Clear();
	void Serialize(CArchive& ar);
	void DataInit();
	long GetItemValue(CString Text);

	CString GetWeldTypeByIndex(long nIdx);
	CString GetImproveTypeByIndex(long nIdx);
	CString GetLayTypeByIndex(long nIdx);
};

#endif // !defined(AFX_STEELWELD_H__646D499E_1FCD_445E_9372_6CBFB4D5D867__INCLUDED_)
