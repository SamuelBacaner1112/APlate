// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BxObject.h: interface for the CPlateBxObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATELINKTOBX_H__194559B3_CDA0_4412_BF9D_A84FE97AB760__INCLUDED_)
#define AFX_PLATELINKTOBX_H__194559B3_CDA0_4412_BF9D_A84FE97AB760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBasicIndex;
class CPlateGirderApp;

// attrubite : m_dwFlag

const DWORD BIT_LK_MODIFY		= 0;
const DWORD BIT_LK_FILL			= 1;
const DWORD BIT_LK_JUSTMODIFIED	= 2;

#define SETDATA(d,s) \
if((d)!=(s)) { (d)=(s); SetDataFilled(TRUE); SetModify(TRUE); SetJustModified(TRUE); }

class AFX_EXT_CLASS CPlateBxObject: public CObject
{
	DECLARE_DYNCREATE(CPlateBxObject)

// Constructor
public:
	CPlateBxObject();
	virtual ~CPlateBxObject();
	friend class CPlateBasicIndex;

// Attributes
protected:
	CPlateGirderApp*		m_pGir;
	POS		m_psBx;
	POS		m_psBxEnd;	// 종방향 부재만 사용 - CJRib, CPlateFactChain
	long	m_nSelfCount;
	DWORD	m_dwFlag;
public:

	enum {	LK_FACTCHAIN_UPPER,	LK_FACTCHAIN_LOWER,LK_FACTCHAIN_WEB,
			LK_VSTIFF, LK_SPLICE, LK_JACKUP, LK_BRACING, LK_HBRACING,
			LK_JIJUM_VSTIFF, LK_WEBHSTIFF,LK_CROSSBEAM, LK_CROSS_BTYPE, 
			LK_CROSS_ATYPE, LK_CROSS_CTYPE, LK_CROSS_PTYPE
		};
// Implementation
public:
	virtual long GetMarkBujeTypical(long nMarkBuje) const { return nMarkBuje; }
	virtual BOOL IsSameJongBujeInMerge(CPlateBxObject* pLK) const { return FALSE; }
	virtual BOOL IsSameByMarkSection(CPlateBxObject* pLK) const { return FALSE; }
	virtual BOOL IsSameByMarkAssem(CPlateBxObject* pLK,long nMarkAssm,long nRow, long nRowLK)
		const { return FALSE; }


	void SetJustModified(BOOL bModify = TRUE);
	BOOL IsJustModified() const;
	void SetModify(BOOL bModify = TRUE);
	BOOL IsModify() const;
	void SetDataFilled(BOOL bFill = TRUE);
	BOOL IsDataFilled() const;

	void SetSelfCount(long nSelfCount)	{ m_nSelfCount = nSelfCount; }
	long GetSelfCount() const			{ return m_nSelfCount; }
	CPlateGirderApp* GetGirder() const		{ return m_pGir; }
	POS GetPOSEnd() const				{ return m_psBxEnd; }
	POS GetPOS() const					{ return m_psBx; }
	CPlateBasicIndex* GetBx() const;
	CPlateBasicIndex* GetBxEnd() const;
	virtual DWORD GetLinkType()			{ return NULL; }
	BOOL IsCreate() const				{ return m_pGir ? TRUE : FALSE; }
	void Create(CPlateGirderApp* pGir, CPlateBasicIndex *pBx);

	void SetPOS(CPlateBasicIndex *pBx);
	void SetPOS(POS psBx)				{ m_psBx = psBx; }
	void SetPOSEnd(POS psBxEnd)			{ m_psBxEnd = psBxEnd; }
	void ClearPOS()						{ m_psBx = NULL; m_psBxEnd = NULL; }

	virtual void Serialize(CArchive& ar);  
	virtual CPlateBxObject& operator=(const CPlateBxObject& obj);
};

#endif // !defined(AFX_PLATELINKTOBX_H__194559B3_CDA0_4412_BF9D_A84FE97AB760__INCLUDED_)
