// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelConcData.h: interface for the CSteelConcData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELCONCDATA_H__D1BCEE63_5036_497A_87F2_55AE3078A417__INCLUDED_)
#define AFX_STEELCONCDATA_H__D1BCEE63_5036_497A_87F2_55AE3078A417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelConcData : public CObject  
{
public:
	CSteelConcData();
	virtual ~CSteelConcData();

// Attributes	
public:
	CString m_strSymbol;		//	���� ��ȣ
	double m_BasicStrength;	//	������� ����
	double	m_SafetyRate;		//	������
	double	m_Alpha;			//	����â ���
	double	m_ElasticModulus;	//	ź�� ���
	double	m_UnitWeight;		//	���� �߷�
	double	m_Temperature;		//	�µ�
	double	m_Epsilon;			//	�������࿡�� ���� ���� ������
	double	m_Creep1;			//	�������࿡�� ���� ũ���������1
	double	m_Creep2;			//	�������࿡�� ���� ũ���������2
	double	m_BendingStress;	//	��� �� ��������
	double	m_ShearStress;		//	��� ��������

// Implementation
public:
	void Serialize(CArchive& ar);
};

#endif // !defined(AFX_STEELCONCDATA_H__D1BCEE63_5036_497A_87F2_55AE3078A417__INCLUDED_)
