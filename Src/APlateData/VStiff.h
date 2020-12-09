// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// VStiff.h: interface for the CVStiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSTIFF_H__3F4F4B1E_DADA_404C_9D28_3CE63C6A4B7F__INCLUDED_)
#define AFX_VSTIFF_H__3F4F4B1E_DADA_404C_9D28_3CE63C6A4B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CPlateBasicIndex;
class CSection;
class AFX_EXT_CLASS CVStiff : public CPlateBxObject  
{
public:
	CVStiff();
	CVStiff(CSection *pSection);
	virtual ~CVStiff() {};

	CSection *m_pSection;
	CPlateBasicIndex *GetBx();

	double m_dThick[2];
	double m_dWidth[2];
	double m_dH[2][2];
	double m_dGap[2];
	BOOL m_cCarryUpper;
	BOOL m_cCarryLower;

	double GetVStiffHeight(BOOL bLeft, double dHGir=-1);
public:
	void Serialize(CArchive &ar);
	CVStiff& operator=(const CVStiff& Obj);
	double m_dLeft;
	double m_dRight;
	long	m_nFlag;
};

#endif // !defined(AFX_VSTIFF_H__3F4F4B1E_DADA_404C_9D28_3CE63C6A4B7F__INCLUDED_)
