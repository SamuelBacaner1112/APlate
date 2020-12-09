// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelSection.h: interface for the CSteelSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELSECTION_H__31066F8D_C228_46BF_AB8E_F16AAD49C97E__INCLUDED_)
#define AFX_STEELSECTION_H__31066F8D_C228_46BF_AB8E_F16AAD49C97E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum	SECTION_DATA_ID
{
	BDISE_TYPE		= 0x00000001,
	BDISE_NAME		= 0x00000002,
	BDISE_SHAPE		= 0x00000004,
	BDISE_CLASS		= 0x00000008,
	BDISE_SYMBOL	= 0x00000010,
	BDISE_SIZETYPE	= 0x00000020,
};

class AFX_EXT_CLASS CSteelSectionData : public CObject  
{
public:
	CSteelSectionData() {};	

// Attributes
public:
	CString	m_strType;		//	�ܸ� ����
	CString	m_strName;		//	�����
	CString	m_strShape;		//	���� ����
	CString	m_strClass;		//	���� ��з�
	CString	m_strSymbol;	//	����
	CString m_strSizeType;	//  ����,����,��Ÿ����

// Implementation
public:
	void Serialize(CArchive& ar)
	{
		int nFlag = 0;
		if(ar.IsStoring()) 
		{
			ar << nFlag;
			ar << m_strType;
			ar << m_strName;
			ar << m_strShape;
			ar << m_strClass;
			ar << m_strSymbol;
			ar << m_strSizeType;
		}
		else 
		{
			ar >> nFlag;
			ar >> m_strType;
			ar >> m_strName;
			ar >> m_strShape;
			ar >> m_strClass;
			ar >> m_strSymbol;
			ar >> m_strSizeType;
			if(m_strSizeType == "��Ÿ����")
				m_strSizeType = "��������";
		}
	}
	CSteelSectionData& operator=(const CSteelSectionData& obj)
	{
		if(this==&obj) return *this;

		m_strType = obj.m_strType;		//	�ܸ� ����
		m_strName = obj.m_strName;		//	�����
		m_strShape = obj.m_strShape;	//	���� ����
		m_strClass = obj.m_strClass;	//	���� ��з�
		m_strSymbol = obj.m_strSymbol;	//	����
		m_strSizeType = obj.m_strSizeType;	//	����,��������
		return *this;
	}
};

class AFX_EXT_CLASS CSteelSection
{
public:
	CSteelSection();
	virtual ~CSteelSection();

// Attributes	
protected:
	CSteelSectionData	m_KeyRec;
	long			m_Key;		//	Search Key;(�˻� ����)
	long			m_idx;		//	Current Search pos
	CTypedPtrArray < CObArray, CSteelSectionData* > m_SectionArr;

// Implementation
public:
	BOOL	m_bSameBuje;	// ���Ϻ��� ��������
	void	Serialize(CArchive& ar);
	void	SerializeFlag1(CArchive& ar, CSteelSectionData *pData);
	void	DataInit();

	void	ClearKey();
	void	AllocKey(CSteelSectionData* pse, long KeyMode);
	BOOL	StartKey();
	BOOL	NextKey();
	CSteelSectionData*	GetRecord(CSteelSectionData* pBS);

	void	RemoveAll();

	long	GetIndex(const CString& strType, const CString& strName) const;
	CString GetStringData(long idx, SECTION_DATA_ID ID) const;
	long	GetSize()  const	{ return m_SectionArr.GetSize();	}

	CString	GetType		(long idx) const;
	CString	GetName		(long idx) const;
	CString	GetShape	(long idx) const;
	CString	GetClass	(long idx) const;
	CString	GetBujeSizeType	(long idx) const;
	
	CString	GetMaterialByMarkBuje(long nMarkBuje) const;
	CString	GetBujeTypeByMarkBuje(long nMarkBuje) const;

	void SetType(long idx,const	CString& strType);
	void SetName(long idx,const	CString& strName);
	void SetShape(long idx,const CString& strShape);
	void SetClass(long idx,const CString& strClass);
	void SetSymbol(long idx,const CString& strSymbol);
	void SetBujeSizeType(long idx,const CString& strSizeType);

};

#endif // !defined(AFX_STEELSECTION_H__31066F8D_C228_46BF_AB8E_F16AAD49C97E__INCLUDED_)
