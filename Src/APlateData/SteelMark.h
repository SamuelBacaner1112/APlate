// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelMark.h: interface for the CSteelMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELMARK_H__69606F22_50AB_44A5_9FCD_5AAD8C310578__INCLUDED_)
#define AFX_STEELMARK_H__69606F22_50AB_44A5_9FCD_5AAD8C310578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelMarkData : public CObject
{
public:
	CSteelMarkData();
	virtual ~CSteelMarkData();
// Attributes
public:
	long	m_nMark[10];
	CString m_sMark[4];
	long	m_nStartNo;
	long	m_nSerialType;
	long	m_nSrBase;
// Implementation
public:
	void Serialize(CArchive& ar);
	CSteelMarkData& operator=(const CSteelMarkData& obj);
};

class AFX_EXT_CLASS CSteelMark  
{
public:
	CSteelMark();
	virtual ~CSteelMark();
// Attributes
public:
	CTypedPtrArray < CObArray, CSteelMarkData* > m_SectionMarkArr;
	CTypedPtrArray < CObArray, CSteelMarkData* > m_PieceMarkArr;
	CTypedPtrArray < CObArray, CSteelMarkData* > m_AssemblyMarkArr;
	CTypedPtrArray < CObArray, CSteelMarkData* > m_MidAssemblyMarkArr;
	CTypedPtrArray < CObArray, CSteelMarkData* > m_BigAssemblyMarkArr;
	CTypedPtrArray < CObArray, CSteelMarkData* > m_WeldMarkArr;
// Implementation
public:
	long GetSectionSize()	{ return m_SectionMarkArr.GetSize();	}
	long GetPieceSize()		{ return m_PieceMarkArr.GetSize();		}
	long GetAssemblySize()	{ return m_AssemblyMarkArr.GetSize();	}
	long GetMidAssemblySize(){ return m_MidAssemblyMarkArr.GetSize();	}
	long GetBigAssemblySize(){ return m_BigAssemblyMarkArr.GetSize();	}
	long GetWeldSize()		{ return m_WeldMarkArr.GetSize();	}
		
	void Serialize(CArchive& ar);
	void SetDataSectionInit();
	void SetDataWeldInit();
	//
	long GetShopSerialType(CString cs);
	CString GetShopSerialType(long nType);
	//
	long GetShopBaseMark(CString cs);
	CString GetShopBaseMark(long nRow, long nCol, long nType);
	//
	long GetShopSrBase(CString cs);
	CString GetShopSrBase(long nType);

	void RemoveAll(long nType);
};

#endif // !defined(AFX_STEELMARK_H__69606F22_50AB_44A5_9FCD_5AAD8C310578__INCLUDED_)
