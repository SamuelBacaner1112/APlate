// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SpliceBase.h: interface for the CPlateSpliceBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATESPLICEBASE_H__EECE5B74_EB78_414F_B1D1_46A84DB27404__INCLUDED_)
#define AFX_PLATESPLICEBASE_H__EECE5B74_EB78_414F_B1D1_46A84DB27404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define     MAXJRIBSU		20

class AFX_EXT_CLASS  CPlateSpliceBase : public CPlateBxObject
{
// Constructor
public:
	CPlateSpliceBase();
	virtual ~CPlateSpliceBase();
// Attributes
public:
	double	m_uA					;		//상부 이음판
	long	m_uB					;
	double	m_uC					;
	double	m_uC1					;
	double	m_uD					;
	double	m_uG					;
	double	m_uT					;
	double	m_uL					;
	double	m_dA					;			//하부 이음판
	long	m_dB					;			//하부 이음판
	double	m_dC					;
	double	m_dC1					;
	double	m_dD					;
	double	m_dG					;
	double	m_dT					;
	double	m_dL					;
	double	m_mA					;			//모멘트 이음판
	long	m_mB					;			//모멘트 이음판
	double	m_mC					;			//모멘트 이음판
	double  m_mC1					;
	double	m_mD					;			// 모멘트이음판 볼트홀~복부판의 연단
	double	m_mG					;
	double	m_mT					;
	double  m_mL					;
	long	m_mE					;
	double	m_mF					;
	double	m_mW					;
	double	m_sA					;			//전단 이음판
	long	m_sB					;			//전단 이음판
	double  m_sC					;
	double  m_sC1					;
	double	m_sT					;
	double	m_sL					;
	double	m_sL1					;			//소수주형 채움판 두께가 9mm이상일경우 볼트 길이...
	long	m_sE					;
	double	m_sF					;
	long	m_sE1					;
	double	m_sF1					;
	double	m_sW					;
	double	m_dBoltSize				;			//m_ZzsBoltSize 모두 수정할 것.
	CString m_ZzsBoltSize			;			//삭제 할 것.
	long	m_uE					;
	double	m_uF					;
	double	m_uW					;
	double	m_uWidth				;
	long	m_dE					;
	double	m_dF					;
	double	m_dW					;
	double	m_dWidth				;
	double  m_mWidth				;
	long	m_nWebType				;			// 0:통판형, 1:모멘트분리형, 2:모멘트일체형

	double	GetuA() const	{ return m_uA;	}		//상부 이음판
	long	GetuB() const	{ return m_uB;	}		//상부 이음판
	double	GetuC	() const { return m_uC	;}
	double	GetuC1() const { return m_uC1;}
	double	GetuD	() const { return m_uD	;}
	double	GetuG	() const { return m_uG	;}
	double	GetuT	() const { return m_uT	;}
	double	GetuL	() const { return m_uL	;}
	double	GetdA	() const { return m_dA	;}
	long	GetdB	() const { return m_dB	;}
	double	GetdC	() const { return m_dC	;}
	double	GetdC1() const { return m_dC1;}
	double	GetdD	() const { return m_dD	;}
	double	GetdG	() const { return m_dG	;}
	double	GetdT	() const { return m_dT	;}
	double	GetdL	() const { return m_dL	;}
	double	GetmA	() const { return m_mA	;}
	long	GetmB	() const { return m_mB	;}
	double	GetmC	() const { return m_mC	;}
	double  GetmC1() const { return m_mC1;}
	double	GetmD	() const { return m_mD	;}
	double	GetmG	() const { return m_mG	;}
	double	GetmT	() const { return m_mT	;}
	double  GetmL	() const { return m_mL	;}
	long	GetmE	() const { return m_mE	;}
	double	GetmF	() const { return m_mF	;}
	double	GetmW	() const { return m_mW	;}
	double	GetsA	() const { return m_sA	;}
	long	GetsB	() const { return m_sB	;}
	double  GetsC	() const { return m_sC	;}
	double  GetsC1() const { return m_sC1;}
	double	GetsT	() const { return m_sT	;}
	double	GetsL	() const { return m_sL	;}
	long	GetsE	() const { return m_sE	;}
	double	GetsF	() const { return m_sF	;}
	long	GetsE1	() const { return m_sE1	;}
	double	GetsF1	() const { return m_sF1	;}
	double	GetsW	() const { return m_sW	;}
	double	GetdBoltSize() const { return m_dBoltSize;}
	CString GetZzsBoltSize() const { return m_ZzsBoltSize;}
	long	GetuE() const { return m_uE; }
	double	GetuF() const { return m_uF; }
	double	GetuW() const { return m_uW; }
	double	GetuWidth() const { return m_uWidth; }
	long	GetdE() const { return m_dE; }
	double	GetdF() const { return m_dF; }
	double	GetdW() const { return m_dW; }
	double	GetdWidth() const { return m_dWidth; }
	double	GetmWidth() const { return m_mWidth;}


	void SetuA(double x) { SETDATA(m_uA,x)	}		//상부 이음판
	void SetuB(long x) { SETDATA(m_uB,x)	}		//상부 이음판
	void SetuC	(double x) { SETDATA(m_uC,x)}
	void SetuC1	(double x) { SETDATA(m_uC1,x)}
	void SetuD	(double x) { SETDATA(m_uD	,x)}
	void SetuG	(double x) { SETDATA(m_uG	,x)}
	void SetuT	(double x) { SETDATA(m_uT	,x)}
	void SetuL	(double x) { SETDATA(m_uL	,x)}
	void SetdA	(double x) { SETDATA(m_dA	,x)}
	void SetdB	(long x) { SETDATA(m_dB	,x)}
	void SetdC	(double x) { SETDATA(m_dC	,x)}
	void SetdC1	(double x) { SETDATA(m_dC1,x)}
	void SetdD	(double x) { SETDATA(m_dD	,x)}
	void SetdG	(double x) { SETDATA(m_dG	,x)}
	void SetdT	(double x) { SETDATA(m_dT	,x)}
	void SetdL	(double x) { SETDATA(m_dL	,x)}
	void SetmA	(double x) { SETDATA(m_mA	,x)}
	void SetmB	(long x) { SETDATA(m_mB	,x)}
	void SetmC	(double x) { SETDATA(m_mC	,x)}
	void SetmC1	(double x) { SETDATA(m_mC1,x)}
	void SetmD	(double x) { SETDATA(m_mD	,x)}
	void SetmG	(double x) { SETDATA(m_mG	,x)}
	void SetmT	(double x) { SETDATA(m_mT	,x)}
	void SetmL	(double x) { SETDATA(m_mL	,x)}
	void SetmE	(long x) { SETDATA(m_mE	,x)}
	void SetmF	(double x) { SETDATA(m_mF	,x)}
	void SetmW	(double x) { SETDATA(m_mW	,x)}
	void SetsA	(double x) { SETDATA(m_sA	,x)}
	void SetsB	(long x) { SETDATA(m_sB	,x)}
	void SetsC	(double x) { SETDATA(m_sC	,x)}
	void SetsC1	(double x) { SETDATA(m_sC1,x)}
	void SetsT	(double x) { SETDATA(m_sT	,x)}
	void SetsL	(double x) { SETDATA(m_sL	,x)}
	void SetsE	(long x) { SETDATA(m_sE	,x)}
	void SetsF	(double x) { SETDATA(m_sF	,x)}
	void SetsE1	(long x) { SETDATA(m_sE1	,x)}
	void SetsF1	(double x) { SETDATA(m_sF1	,x)}
	void SetsW	(double x) { SETDATA(m_sW	,x)}
	void SetdBoltSize(double x) { SETDATA(m_dBoltSize,x)}
	void SetZzsBoltSize(CString x) { SETDATA(m_ZzsBoltSize,x)}
	void SetuE(long x) { SETDATA(m_uE,x) }
	void SetuF(double x) { SETDATA(m_uF,x) }
	void SetuW(double x) { SETDATA(m_uW,x) }
	void SetuWidth(double x) { SETDATA(m_uWidth,x) }
	void SetdE(long x) { SETDATA(m_dE,x) }
	void SetdF(double x) { SETDATA(m_dF,x) }
	void SetdW(double x) { SETDATA(m_dW,x) }
	void SetdWidth(double x) { SETDATA(m_dWidth,x) }
	void SetmWidth(double x) { SETDATA(m_mWidth,x)}

public:
	BOOL IsTrue();
	
	double GetuL(double thickBase) const;
	double GetdL(double thickBase) const;	
	double GetsL(double thickBase) const;
	double GetmL(double thickBase) const;

	void SerializeSpliceBase(CArchive& ar);  
	CPlateSpliceBase& operator=(const CPlateSpliceBase&	Obj);
	BOOL operator==( const CPlateSpliceBase& Obj);
};


#endif // !defined(AFX_PLATESPLICEBASE_H__EECE5B74_EB78_414F_B1D1_46A84DB27404__INCLUDED_)
