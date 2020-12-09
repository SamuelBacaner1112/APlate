// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelUWeight.h: interface for the CSteelUWeight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELUWEIGHT_H__CBAD85EA_BE06_4845_B64A_F25E4BD2E0E1__INCLUDED_)
#define AFX_STEELUWEIGHT_H__CBAD85EA_BE06_4845_B64A_F25E4BD2E0E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelUWeight : public CObject  
{
public:
	CSteelUWeight();
	virtual ~CSteelUWeight();

public:
	double m_dUWeightGangjae;			///< ��ö �����߷�
	double m_dUWeightAluminum;			///< �˹̴� �����߷�
	double m_dUWeightChulgunCon;		///< ö�� ��ũ��Ʈ �����߷�
	double m_dUWeightMugunCon;			///< ���� ��ũ��Ʈ �����߷�
	double m_dUWeightCement;			///< �ø�Ʈ ���� �����߷�
	double m_dUWeightMokjae;			///< ���� �����߷�
	double m_dUWeightYukchung;			///< ��û��(�����) �����߷�
	double m_dUWeightAsphalt;			///< �ƽ���Ʈ ���� �����߷�
	double m_dUWeighGravel;				///< �ڰ�,�⼮ �����߷�
	long	m_nTypePave;
	CString GetTypePave();
	long	GetTypePave(CString strTypePave);
	double	GetPaveUnitWeight(CString strTypePave);
public:
	void Serialize(CArchive& ar);
	void DataInit();
};

#endif // !defined(AFX_STEELUWEIGHT_H__CBAD85EA_BE06_4845_B64A_F25E4BD2E0E1__INCLUDED_)
