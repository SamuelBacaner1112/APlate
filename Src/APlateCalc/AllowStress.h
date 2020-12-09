// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// AllowStress.h: interface for the CAllowStress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLOWSTRESS_H__84F60A24_DA8D_4293_8DD4_CA070A37B9DC__INCLUDED_)
#define AFX_ALLOWSTRESS_H__84F60A24_DA8D_4293_8DD4_CA070A37B9DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutCalcStd;
#define ALLOW_QTY_MAX					3		// �β� ����

#define ALLOW_SHEARSTRESS				0		// �����������
#define ALLOW_TENSIONSTRESS				1		// �����������
#define ALLOW_COMPSTRESS				2		// ����������
#define ALLOW_BUCKLINGCOEFF				3		// ������������ �����±��� ���� ������� ���
#define ALLOW_BUCKLING					4		// ������������ �����±��� ���� �������
#define ALLOW_STDYIELDPOINT_UP			5		// �ǵβ��� ���� �����׺��� - ����
#define ALLOW_STDYIELDPOINT_LO			6		// �ǵβ��� ���� �����׺��� - ����
#define ALLOW_BENDINGCOMP_CASE1			7		// ��� �ھ������� - �����÷����� ��ũ��Ʈ�ٴ��� � ���������Ǿ� �ִ°�� Ⱦ�±��� �Ͼ�� �������(Box ��)
#define ALLOW_BENDINGCOMP_CASE2			8		// ��� �ھ������� - ��� �� ���� ���(����)
#define ALLOW_BUCKLING_UNCONSIDER		9		// �����±��� ������� ���� �������� ��������
#define ALLOW_FLANGE_SPACE				10		// ���������縦 ������ �� �ִ� �÷��� �������� �ִ�ġ
#define ALLOW_WEBMINTHICK				11		// ������ �ּҵβ�
#define ALLOW_STDYIELDPOINT_WEB			12		// �ǵβ��� ���� �����׺��� - ������

#define THICK40BELOW					0		// �β� 40����
#define THICK75BELOW					1		// �β� 40�ʰ� 75����
#define THICK100BELOW					2		// �β� 75�ʰ� 100����

typedef	struct AFX_EXT_CLASS _ALLOWSTRESS
{
	CString	m_strMaterial;
	double	m_dAllowStress;
	double	m_dCoeff1;
	double	m_dCoeff2;
	double	m_dCoeff3;
	double	m_dCoeff4;
	double	m_dCoeff5;
} ALLOWSTRESS;

class AFX_EXT_CLASS CAllowStress  
{
public:
	CAllowStress(long nTypeAllow, long nMaterial);
	virtual ~CAllowStress();

	void InitData();
	double GetAllowStress(CString strMaterial, double dThick);
	long GetTypeAllow()				{ return m_nTypeAllow;		};
	long GetIdxByThick(double dThick);
	long GetSteelSize()				{ return m_nSize;			};
	long GetThickSize()				{ return m_nThickSize;		};
	double GetDistFlangeFixed()		{ return m_dDistFlangeFixed;};
	double GetWidthFlange()			{ return m_dWidthFlange;	};
	double GetAreaWeb()				{ return m_dAreaWeb;		};
	double GetAreaConc()			{ return m_dAreaConc;		};
	long GetMaterialIndex(CString strMaterial);
	CString GetAllowStressTitle();
	long GetDecimalCiphers();

	void InsertItem(long nThickIdx, CString strMaterial, double dAllowStress, double dAllowCoeff1=0.0, double dAllowCoeff2=0.0, double dAllowCoeff3=0.0, double dAllowCoeff4=0.0, double dAllowCoeff5=0.0);
	void SetAllowBendingStress(double dAw, double dAc, double dDistFlangeFixed, double dWidthFlange);
	void SetAllowBucklingUnconsider(double dLengthBuckling, double dTurnigRadius);
	void SetAllowBuckling(double dThick, double dWidth);
	vector<ALLOWSTRESS>	m_vtAllowStress[ALLOW_QTY_MAX];

private:
	long	m_nTypeAllow;
	long	m_nMaterial;
	long	m_nSize;
	long	m_nThickSize;
	CString	m_strMaterial;
	double	m_dAreaWeb;					// �������� �Ѵܸ���(mm2)
	double	m_dAreaConc;				// �����÷��� �Ѵܸ���(mm2)
	double	m_dDistFlangeFixed;			// �����÷��� ���������� �Ÿ�(mm)
	double	m_dWidthFlange;				// �����÷��� ��(mm)
	double	m_dK;						// sqrt(3+Aw/2Ac)
	double	m_dLengthBuckling;			// ������ ��ȿ�±�����
	double	m_dTurningRadius;			// ���� �Ѵܸ��� �ܸ�ȸ���ݰ�
	double	m_dThick;					// Material �β�
	double	m_dWidth;					// Material ��

	void ClearStress();
};

#endif // !defined(AFX_ALLOWSTRESS_H__84F60A24_DA8D_4293_8DD4_CA070A37B9DC__INCLUDED_)
