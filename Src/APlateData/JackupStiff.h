// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// JackupStiff.h: interface for the CJackupStiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JACKUPSTIFF_H__5CEF27A9_1AB0_4810_9A3C_48B864EEDAE9__INCLUDED_)
#define AFX_JACKUPSTIFF_H__5CEF27A9_1AB0_4810_9A3C_48B864EEDAE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
class AFX_EXT_CLASS CJackupStiff  : public CPlateBxObject
{
// Constructor
public:
	CJackupStiff();
	virtual ~CJackupStiff();

// Attributes
public:	
	CVector	m_vJewon[2];
	double m_dVL;
	double m_dVG;

// Implementation
public:
	void CreatePlatePieceObj(CPlatePieceObject *pObj);
	BOOL IsSameByMarkSection(CPlateBxObject* pLK, BOOL bLeft) const;
	virtual DWORD GetLinkType();
	double GetHeight(BOOL bLeft) const;
	double GetThick(BOOL bLeft) const;
	double GetWidth(BOOL bLeft) const;
	CVector GetJewon(BOOL bLeft) const;		
	void Serialize(CArchive& ar);
	CJackupStiff& operator=(const CJackupStiff& obj);

};

#endif // !defined(AFX_JACKUPSTIFF_H__5CEF27A9_1AB0_4810_9A3C_48B864EEDAE9__INCLUDED_)
