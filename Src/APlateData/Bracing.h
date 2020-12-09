// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Bracing.h: interface for the CBracing class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BRACING_H__A084084B_13B8_4EFD_8293_961EE4565967__INCLUDED_)
#define AFX_BRACING_H__A084084B_13B8_4EFD_8293_961EE4565967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "HBracingGusset.h"

class CPlatePieceObject;
class AFX_EXT_CLASS CBracing : public CPlateBxObject  
{
public:
	CBracing();
	virtual ~CBracing();

private:
	long m_nType;
	CSteelMaterialData	*m_pSteelMaterialData;
public:	
	enum { LEFTUP, LEFTDN, RIGHTUP, RIGHTDN, MIDDLE };
	enum { TOP, BOTTOM, LEFTCROSS, RIGHTCROSS };	

	double m_dLengthBeam[2];
	long m_nStdSteelIdx;
public:
	enum { BRACTYPE_NONE, 
		   BRACTYPE_LEFTWAVE, BRACTYPE_RIGHTWAVE, 
		   BRACTYPE_FRONTWAVE, BRACTYPE_BACKWAVE,
		   BRACTYPE_CROSS,
		   BRACTYPE_SLOPLEFT, BRACTYPE_SLOPRIGHT,
		   BRACTYPE_CROSSLEFT, BRACTYPE_CROSSRIGHT
	};	
	enum {H, B, T1, T2, R1, R2, LENGTH};	

public:
	void SetType(long type) { m_nType = type; }
	long GetType()			{ return m_nType; }
public:

	virtual void Serialize(CArchive &ar);
	void SerializeForOld(CArchive &ar);
	double GetBeamJewon(long nPos, BOOL bUpper, long nJewon);

	CSteelMaterialData* GetSteelMaterialData()	{return m_pSteelMaterialData;};
	void SetSteelMaterialData(CSteelMaterialData *pSteelMaterialData)	{m_pSteelMaterialData=pSteelMaterialData;};
};

#endif // !defined(AFX_BRACING_H__A084084B_13B8_4EFD_8293_961EE4565967__INCLUDED_)
