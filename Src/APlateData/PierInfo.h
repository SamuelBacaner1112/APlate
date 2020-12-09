// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PierInfo.h: interface for the CPierInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIERINFO_H__417BBEFA_D666_4043_BFCF_3840F49012AB__INCLUDED_)
#define AFX_PIERINFO_H__417BBEFA_D666_4043_BFCF_3840F49012AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// m_dwFlag bit
// 0 : Abut=0, Pier=1
// 1 : Use Left R 
// 2 : Use Right R 

class AFX_EXT_CLASS CPierInfo  
{
// Constructor
public:
	CPierInfo(CPierInfo* pPierInfo = NULL);
	virtual ~CPierInfo();

public:		
		// 교대
		double m_daT1;
		double m_daT2;
		double m_daT3;
		double m_daT4;
		double m_daH1;
		double m_daH2;
		double m_daH3;
		double m_daH4;
		double m_daH5;
		double m_daH6;
		double m_daH7;
		double m_daH8;
		double m_daB1;
		double m_daB2;
		double m_daB3;
		double m_daK1;
		double m_daK2;

		double m_daW;
		double m_daW1;
		double m_daW2;		
		
		double m_daWL;
		double m_daWT;	

		// 교각
		double m_dpB1;
		double m_dpB2;
		double m_dpB3;
		double m_dpB4;
		double m_dpD;
		double m_dpP;	
		double m_dpN;	
		double m_dpW;
		double m_dpH1;
		double m_dpH2;
		double m_dpK1;
		double m_dpK2;
		double m_dpH3;
		double m_dpH4;

		double m_dpR;
		
// Attributes
private:
	DWORD	m_dwFlag;
	long	m_nNumberPier;
	//double	m_dWidth;
	//double	m_dALeft;
	//double	m_dARight;

// operator
	CPierInfo& operator = (const CPierInfo& obj);


// Implementation
public:
//	BOOL IsValid() const { return m_dWidth > 0 ? TRUE : FALSE; }
	void Serialize(CArchive& ar);

	double  GetHorWidth() const  { return IsAbut() ? m_daWL : m_dpW + m_dpR * 2; }
	double	GetWidth() const  {	return IsAbut() ? m_daT4 : m_dpP + m_dpN;	}
	double	GetHeight() const { return IsAbut() ? m_daH4 : m_dpH3; }
	//double	GetALeft() const { return m_dALeft;}
	//double	GetARight() const { return m_dARight;}
	//void SetWidth(double dWidth)  	{	 m_dWidth = dWidth;}
	//void SetALeft(double A)  	{  m_dALeft = A;}
	//void SetARight(double A)  	{  m_dARight = A;}
	long GetNumberPier() const { return m_nNumberPier; }
	void SetNumberPier(long nPier)  	{  m_nNumberPier = nPier;}

	//void SetLeftR(BOOL bUse);
	//void SetRightR(BOOL bUse);
	//BOOL GetLeftR() const;
	//BOOL GetRightR() const;
	void SetAbut(BOOL bAbut);
	BOOL IsAbut() const;

	friend CArchive& operator <<( CArchive& ar, CPierInfo& v){ v.Serialize(ar); return ar;}
	friend CArchive& operator >>( CArchive& ar, CPierInfo& v){	v.Serialize(ar); return ar;}
};

#endif // !defined(AFX_PIERINFO_H__417BBEFA_D666_4043_BFCF_3840F49012AB__INCLUDED_)
