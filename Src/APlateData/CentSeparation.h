// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// CentSeparation.h: interface for the CCentSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CENTSEPARATION_H__C0116D1E_F00B_47E8_8122_FBF4B5E6FA9B__INCLUDED_)
#define AFX_CENTSEPARATION_H__C0116D1E_F00B_47E8_8122_FBF4B5E6FA9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AREA_NUM	7

//��ȣ�� ����, ö������ ���, ���̾�α׿��� �Է� ���� ��� ���
#define  HT_NONE				0		//����
#define  HT_GUARDFENCE			1		//��ȣ��
#define	 HT_CENTERGUARDFENCE	2		//�ߺд�
#define  HT_PEOPLEROAD			3		//����
//
//#define  HT_DETECTGRAVEL	2		//�ڰ�����
//#define  HT_VERDOSANG		4		//����ĵ���
//#define  HT_GRAVELDOSANG	5		//�ڰ�����
//#define  HT_GUARDFENCEANDROAD 6		//����+����
//#define  HT_ELECHOLE		7		//��ű�
//#define  HT_ELECHOLEANDROAD 8		//��ű�+����

//��ȣ�� ����, CCentSeparation ��ü�� �����Ͽ� ���
#define  SLAB_GUARD_LEFT		0		//��ȣ��(��)		
#define  SLAB_GUARD_RIGH		1		//��ȣ��(��)		
#define  SLAB_GUARD_CENT		2		//�ߺд�			
#define  SLAB_CURB_LEFT			3		//����(��)			
#define  SLAB_CURB_RIGH			4		//����(��)			
#define  SLAB_GRAVEL_LEFT		5		//�ڰ�����(��)		
#define  SLAB_GRAVEL_RIGH		6		//�ڰ�����(��)		
#define  SLAB_CURB_WALK_LEFT	7		//����+����(��)		
#define  SLAB_CURB_WALK_RIGH	8		//����+����(��)		
#define  SLAB_CABLE_LEFT		9		//��ű�(��)		
#define  SLAB_CABLE_RIGH		10		//��ű�(��)		
#define  SLAB_CABLE_WALK_LEFT	11		//��ű�+����(��)	
#define  SLAB_CABLE_WALK_RIGH	12		//��ű�+����(��)	

class CGuardFenceRebar;

class AFX_EXT_CLASS CCentSeparation : public CObject
{
public:
	CCentSeparation();
	virtual ~CCentSeparation();
	enum PLACE	{ LEFT, CENTER, RIGHT } nPlace;

	long	GetPlace();
	double	GetWidth() const;
	double	GetHeight() const;
	double	GetDistCentroid();
	void	GetXyGuardFence(CDPoint A[9]) const;
	void	SetDataDefault1(CCentSeparation *pGuard, long nGuard , long bLeft);

	double	GetArea();
	void	InitCalculate(double dUnitWeight = 2.5);

public:
	
	void SetSeperationType(CString str);
	// void SetInitData();
	CString		m_strName;
	double		m_D1;
	double		m_W1;
	double		m_W2;
	double		m_W3;
	double		m_W4;
	double		m_W5;
	double		m_H1;
	double		m_H2;
	double		m_H3;
	long		m_nSepType;
	long		m_nDirType;

	double		m_DistCentroid;	
	double		m_Area[AREA_NUM];			// �ִ� ������ ���� (7����)
	double		m_DistArea[AREA_NUM];		// �� ������ ���� ���ɱ����� �Ÿ� (�������� ���� �ϴ����� ������ �Ÿ�)
	vector<CDPoint> m_dAreaCenter;			// ������ �߾���
	vector<pair<CString, CDPoint> > m_strAreaFormula;	// ������ ���� ���

	void Serialize( CArchive& ar );
	void SerializeOld( CArchive& ar );
	CCentSeparation& operator=(const CCentSeparation& obj);

public:
	CGuardFenceRebar* GetRebar();

private:
	CGuardFenceRebar* m_pRebar;
};

#endif // !defined(AFX_CENTSEPARATION_H__C0116D1E_F00B_47E8_8122_FBF4B5E6FA9B__INCLUDED_)
