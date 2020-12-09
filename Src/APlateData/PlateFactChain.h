// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateFactChain.h: interface for the CPlateFactChain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEFACTCHAIN_H__CA8FFD07_8E6B_456D_ABA5_3FCB21587FF6__INCLUDED_)
#define AFX_PLATEFACTCHAIN_H__CA8FFD07_8E6B_456D_ABA5_3FCB21587FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�β��� ���� ������ CPlateFactChain�� �ִ�.
class CPlateFactChain : public CPlateBxObject  
{
public:
	CPlateFactChain();
	virtual ~CPlateFactChain();
	
public:
	virtual void Serialize(CArchive& ar);
	double GetWidth() const 
	{ return m_dWidth;}
	void   SetWidth(double dWidth)
	{ m_dWidth = dWidth;}
	double GetWidthOut() const 
	{ return m_dWidthOut;}
	void   SetWidthOut(double dWidthOut)
	{ m_dWidthOut = dWidthOut;}
	CString GetSymbol() const { return m_strSymbol; }
	
// Attributes
public:
	// ��������
	DWORD  m_nMarkBujeFact;			// 0:���,1:�Ϻ�,2:����������, 3:����������
	double m_dWidth;				// ��	
	double m_dWidthOut;				// ǫ(�ٱ���)
	double m_dFactChainThick;		// �β�
	double m_dFactChainThickOut;	// �β�(�ٱ���)
	CString	m_strSymbol;	//	����

// Implementation
public:
	CPlateBasicIndex* GetBxOnlyFact(BOOL bStt) const;
	BOOL IsSameJongBujeInMerge(CPlateBxObject* pLK) const;
	BOOL IsSpliceInFactChain(BOOL bStt) const;
	BOOL IsSameByMarkSection(CPlateBxObject* pLK) const;
	virtual DWORD GetLinkType();	 
	CPlateFactChain& operator=(const CPlateFactChain&Obj);
	void GetBoltHoleFlange(CVectorArray &vArr, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bSttDir) const;
	void GetBoltHoleWeb(CVectorArray &vArr, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bSttDir) const;
		
public:
	void CreatePieceObj(CPlatePieceObject *pObj) const;
	void CreatePieceObject3D(CPlatePieceObject *pObj) const;		

};

#endif // !defined(AFX_PLATEFACTCHAIN_H__CA8FFD07_8E6B_456D_ABA5_3FCB21587FF6__INCLUDED_)
