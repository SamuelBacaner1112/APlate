// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Section.h: interface for the CSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTION_H__57FF395C_EF3E_4E81_A556_BE1E4C6CE4A6__INCLUDED_)
#define AFX_SECTION_H__57FF395C_EF3E_4E81_A556_BE1E4C6CE4A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateCrossBeam;
class CVBracing;
class CVStiff;

#define SECDET_CROSSBEAM	0		// 가로보 타입
#define SECDET_VBRACING		1		// 브레이싱 타입
#define SECDET_VSITFF		2		// 수직보강재 부착 타입

#define SECTION_A			0
#define SECTION_P			1
#define SECTION_B			2
#define SECTION_C			3
#define SECTION_V			4
#define SECTION_J			5

class AFX_EXT_CLASS CSection : public CPlateBxObject  
{
public:
	CSection();
	virtual ~CSection();

	BOOL m_bSetType;	//저장 필요없음.

private:
	long m_enType;
	long m_enTypeNum;
	long m_enDetType;
	CDPoint m_vAngLeft;
	CDPoint m_vAngRight;
	double	m_dStationLeft;				// 거더 중심부터 이격된 가로보 Station
	double	m_dStationRight;			// 거더 중심부터 이격된 가로보 Station


public:
	void Serialize(CArchive &ar);

	void SetenType(long nType)				{ m_enType = nType; }
	void SetenTypeNum(long nTypeNum)		{ m_enTypeNum = nTypeNum; }	
	void SetenDetType(long nType);

	long GetenType()					{ return m_enType; }
	long GetenTypeNum()					{ return m_enTypeNum; }
	long GetenDetType()					{ return m_enDetType; }

	long IsenType(long nType)			{ return (nType == m_enType); }	
	long IsenDetType(long nType)		{ return (nType == m_enDetType); }	
	
	BOOL IsSetDetType()					{ return (m_enDetType != -1); }
	union 
	{
		CPlateCrossBeam *m_pCrossBeam;
		CVBracing		*m_pVBracing;
		CVStiff			*m_pVStiff;
	};
private:
	long m_nSecType;

public:
	enum { H, B, T1, T2, R1, R2, LENGTH, CENTERDX, CENTERDY};
	enum { SEC_UP, SEC_DOWN, SEC_LEFT, SEC_RIGHT, SEC_UHBEAM, SEC_LHBEAM};//상현, 하현, 사현

	CPlateCrossBeam *GetCrossBeam() { return m_enDetType == SECDET_CROSSBEAM ? m_pCrossBeam : NULL; }
	CVBracing		*GetVBracing()  { return m_enDetType == SECDET_VBRACING ? m_pVBracing : NULL; }
	CVStiff			*GetVStiff();
	CString m_strCalcSectionName;	//모델링에서 가로보 및 수직브레이싱의 단면이름

	//////////////////////////////////////////////////////////////////////////////
	double GetBeamJewon(long nPos, long nJewon);
	double GetBeamBaseHeight(long nPos, BOOL bVBracing, BOOL bFront);
};

#endif // !defined(AFX_SECTION_H__57FF395C_EF3E_4E81_A556_BE1E4C6CE4A6__INCLUDED_)
