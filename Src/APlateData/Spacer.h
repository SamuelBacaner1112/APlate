// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Spacer.h: interface for the CSpacer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPACER_H__7A1C8A25_A923_440C_9563_0C1D4C44BA7A__INCLUDED_)
#define AFX_SPACER_H__7A1C8A25_A923_440C_9563_0C1D4C44BA7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSpacer : public CObject
{
private:
	double		m_dRebarDiameter;			//철근직경
	double		m_dRebarLength_A;			//간격제 제원중 A의 길이
	double		m_dRebarLength_B;			//간격제 제원중 B의 길이
	double		m_dRebarLength_C;			//간격제 제원중 C의 길이
	double		m_dRebarLength_D;			//간격제 제원중 D의 길이
	int			m_nRebarCalcType;			//철근제원 산출 방법 종류 0:배치간격, 1:1개의 간격....
	int			m_nRebarCntInSquare;		//1m^2당 간격제의 개수
	int			m_nCntMainRebarSet;			//1개의 철근이 감당하는 주철근 간격 
	int			m_nCntDistRebarSet;			//1개의 철근이 감당하는 배력철근 간격 
	double		m_dRebarLength_E;			//기타 길이...
	double		m_dFy;						//철근 항복강도 

public:
	CSpacer();
	CSpacer(const CSpacer& obj);
	virtual ~CSpacer();

//Method
public:
	void	SetDataDefault(BOOL bRecommend);
	void	Serialize(CArchive& ar);
	void	SetRebarDiameter(double dDia){m_dRebarDiameter = dDia;}
	double	GetRebarDiameter(){return m_dRebarDiameter;}
	void	SetRebarLength_A(double dLen){m_dRebarLength_A = dLen;}
	double	GetRebarLength_A(){return m_dRebarLength_A;}
	void	SetRebarLength_B(double dLen){m_dRebarLength_B = dLen;}
	double	GetRebarLength_B(){return m_dRebarLength_B;}
	void	SetRebarLength_C(double dLen){m_dRebarLength_C = dLen;}
	double	GetRebarLength_C(){return m_dRebarLength_C;}
	void	SetRebarLength_D(double dLen){m_dRebarLength_D = dLen;}
	double	GetRebarLength_D(){return m_dRebarLength_D;}
	void	SetRebarLength_E(double dLen){m_dRebarLength_E = dLen;}
	double	GetRebarLength_E(){return m_dRebarLength_E;}
	void	SetRebarCalcType(int nType){m_nRebarCalcType = nType;}
	int		GetRebarCalcType(){return m_nRebarCalcType;}
	void	SetCntMainRebarSet(int nCnt){m_nCntMainRebarSet = nCnt;}
	int		GetCntMainRebarSet(){return m_nCntMainRebarSet;}
	void	SetCntDistRebarSet(int nCnt){m_nCntDistRebarSet = nCnt;}
	int		GetCntDistRebarSet(){return m_nCntDistRebarSet;}
	void	SetRebarCntInSquare(int nCnt){m_nRebarCntInSquare = nCnt;}
	int		GetRebarCntInSquare(){return m_nRebarCntInSquare;}
	void	SetFy(double dFy){m_dFy = dFy;}
	double  GetFy(){return m_dFy;}

};

#endif // !defined(AFX_SPACER_H__7A1C8A25_A923_440C_9563_0C1D4C44BA7A__INCLUDED_)
