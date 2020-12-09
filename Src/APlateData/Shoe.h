// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Shoe.h: interface for the CShoe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOE_H__0BAB936E_BA9E_4D9B_B0D8_5AB762BC6A20__INCLUDED_)
#define AFX_SHOE_H__0BAB936E_BA9E_4D9B_B0D8_5AB762BC6A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CShoe : public CObject
{
// Construtor
public:
	CShoe();
	CShoe(const CShoe& shoe);
	virtual ~CShoe();
protected:
	double		m_dDisShoeFrWeb;		//��.�� �����ǿ����� �Ÿ�	//�ڽ����� �Է� m_dDisShoeFrWeb
// Attributes
public:
	BYTE		m_cDirectionShoe;		//�� ����				//�������� �Է�, ����, �����, ������, Ⱦ���� ��
	double		m_ZzdAngleShoe;			//������ġ ����			//�������� �Է� ������ ��.
	BYTE		m_bLevelSolePlate;		//���� ����				//���÷���Ʈ �Է� m_bLevelSolePlate
	double		m_dW_SolePlate;			//������ ����			//���÷���Ʈ �Է� m_dW_SolePlate
	double		m_dW2_SolePlate;		//������ ����			//���÷���Ʈ �Է� m_dW2_SolePlate
	double		m_dW3_SolePlate;		//������ ����			//���÷���Ʈ �Է� m_dW3_SolePlate
	double		m_dH_SolePlate;			//�������� ����			//���÷���Ʈ �Է� m_dH_SolePlate
	double		m_dH2_SolePlate;		//�������� ����			//���÷���Ʈ �Է� m_dH2_SolePlate
	double		m_dH3_SolePlate;		//�������� ����			//���÷���Ʈ �Է� m_dH3_SolePlate
	double		m_dDia_SolePlateHole;	//Ȧ������				//���÷���Ʈ �Է� m_dDia_SolePlateHole
	BOOL		m_bHole_SolePlate;		//Ȧ �Ǵ� �� ����		//���÷���Ʈ �Է� m_bHole_SolePlate
	double		m_dThick_SolePlate;		//�߾Ӻ� �β�			//���÷���Ʈ �Է� m_dThick_SolePlate
	double		m_dThick_Shoe;			//SHOE�� �β�			//���÷���Ʈ �Է� m_dThick_Shoe
	double		m_dThick_Mortar;		//�������Ż�� �β�		//���÷���Ʈ �Է� m_dThick_Mortar
	double		m_dThick_Block;			//SHOE BLOCK�� ����		//���÷���Ʈ �Է� m_dThick_Block
	long		m_dForceShoe;			//SHOE�� ���� ���ݷ�	//���÷���Ʈ �Է� m_dForce
	double		m_dW1_Mortal;			//��Ż�� W1 - ���� m_dW1_Mortal
	double		m_dW2_Mortal;			//��Ż�� W2 - ���� m_dW2_Mortal
	double		m_dW3_Mortal;			//��Ż�� W3 - ���� �ݴ� m_dW3_Mortal
	CVector		m_vXyzShoeInput;		// ���� ���� ��ǥ (�����ǥ�� �ƴ�, ���� �Ǵ� ������, �Էµ� ��ǥ)
	long		m_nRefType;				// DB ������ ���

// Access method
	double	GetDisShoeFrWeb() const	{ return 	m_dDisShoeFrWeb;	}
	BYTE	GetDirectionShoe() const				{ return 	m_cDirectionShoe;	}
	BYTE	GetLevelSolePlate() const		{ return 	m_bLevelSolePlate;	}
	double	GetWidthTotalSolePlate() const	{ return 	m_dW_SolePlate + m_dW2_SolePlate*2;	}
	double	GetWSolePlate() const		{ return 	m_dW_SolePlate;	}
	double	GetW2SolePlate() const		{ return 	m_dW2_SolePlate;	}
	double	GetW3SolePlate() const		{ return 	m_dW3_SolePlate;	}
	double	GetHeigthTotalSolePlate() const	{ return 	m_dH_SolePlate + m_dH2_SolePlate*2;	}
	double	GetHSolePlate() const		{ return 	m_dH_SolePlate;	}
	double	GetH2SolePlate() const		{ return 	m_dH2_SolePlate;	}
	double	GetH3SolePlate() const		{ return 	m_dH3_SolePlate;	}
	double  GetDiaSolePlateHole() const { return  m_dDia_SolePlateHole; }
	BOOL	IsHoleSolePlate() const				{ return	m_bHole_SolePlate;	}
	double	GetThickSolePlate() const		{ return 	m_dThick_SolePlate;	}
	double	GetThickShoe() const				{ return 	m_dThick_Shoe;	}
	double	GetThickMortar() const				{ return 	m_dThick_Mortar;	}
	double	GetThickBlock() const				{ return 	m_dThick_Block;	}
	double	GetThickTotalShoeSet() const		{ return 	m_dThick_SolePlate+m_dThick_Shoe+m_dThick_Mortar+m_dThick_Block;	}
	long	GetForceShoe() const				{ return 	m_dForceShoe;	}
	CVector	GetXyzShoeInput() const				{ return 	m_vXyzShoeInput;	}

	void	SetDisShoeFrWeb(double Dis) 		{ m_dDisShoeFrWeb = Dis;	}
	void	SetDir(BYTE Dir) 					{ m_cDirectionShoe = Dir;	}
	void	SetbLevelApply(BYTE bLevelApply) 	{ m_bLevelSolePlate = bLevelApply;	}
	void	SetSolePlate_W(double SolePlate_W) 	{ m_dW_SolePlate = SolePlate_W;	}
	void	SetSolePlate_W2(double SolePlate_W2){ m_dW2_SolePlate = SolePlate_W2;	}
	void	SetSolePlate_W3(double SolePlate_W3){ m_dW3_SolePlate = SolePlate_W3;	}
	void	SetSolePlate_H(double SolePlate_H) 	{ m_dH_SolePlate = SolePlate_H;	}
	void	SetSolePlate_H2(double SolePlate_H2){ m_dH_SolePlate = SolePlate_H2;	}
	void	SetSolePlate_H3(double SolePlate_H3){ m_dH_SolePlate = SolePlate_H3;	}
	void	SetSolePlate_T(double SolePlate_T) 	{ m_dThick_SolePlate = SolePlate_T;	}
	void	SetSolePlate_HoleSize(double HoleSize) { m_dDia_SolePlateHole = HoleSize; }
	void	SetHole(BOOL bValue)				{ m_bHole_SolePlate = bValue; }
	void	SetH2(double H2) 					{ m_dThick_Shoe = H2;	}
	void	SetH3(double H3) 					{ m_dThick_Mortar = H3;	}
	void	SetH4(double H4) 					{ m_dThick_Block = H4;	}
	void	SetTon(long ton) 					{ m_dForceShoe = ton;	}

//	implementation
public:
	BOOL IsExist() const;
	double GetHeightTotalShoe() const;
	void SerializeShoe(CArchive& ar);
	void SerializeForOldVersion(CArchive& ar);
	CShoe& operator=(const CShoe& Obj);
	BOOL operator==( const CShoe& Obj) const;
	BOOL operator!=( const CShoe& Obj) const	{	return !(*this == Obj); }
};


#endif // !defined(AFX_SHOE_H__0BAB936E_BA9E_4D9B_B0D8_5AB762BC6A20__INCLUDED_)
