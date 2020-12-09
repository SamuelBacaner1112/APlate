// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DesignStiff.h: interface for the CDesignJijumStiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESIGNJIJUMSTIFF_H__DF7A5263_55A4_4A08_9989_CE35D78938C4__INCLUDED_)
#define AFX_DESIGNJIJUMSTIFF_H__DF7A5263_55A4_4A08_9989_CE35D78938C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ������ ������ - Web������(����.����)
class AFX_EXT_CLASS CDesignStiff : public CObject
{
public:
	CDesignStiff(CAPlateCalcStd *pAPlateCalcStd);
	virtual ~CDesignStiff();

	void SetVerStiff(CPlateBasicIndex *pBx);
	void SetHorStiff(CPlateBasicIndex *pBx);
	
	struct _VERSTIFF
	{
		CString	m_strMaterial;		//���������� ����
		double	m_dGap;				//���������� ���� a
		double	m_dMaxA_B;			//���ݺ� �ִ�..
		double	m_dUseA_B;			//��� ���ݺ�..
		double	m_dUseW;			//��� ���������� �� bvs
		double	m_dReqW;			//�ʿ� ���������� ��
		double	m_dUseT;			//��� ���������� �β� tvs		
		double	m_dReqT;			//�ʿ� ���������� �ּҵβ�
		double	m_dFlangeGap;		//���Ͼ��÷����� ������ b
		double	m_dWebThick;		//�������� �β� t
		double	m_dUseI;			//��� ����..
		double	m_dReqI;			//�ʿ� ����..
		
	} m_strtVerStiff;

	struct _HORSTIFF
	{
		CString	m_strMaterial;		//���򺸰��� ����
		double	m_dGap;				//���������� ���� a
		long	m_lUse_EA;			//���򺸰��� ��� ����
		double  m_dUseWebT;			//������ ���β�
		double  m_dReqWebT;			//������ �ʿ�β�..
		double	m_dUseW;			//��� ���������� �� bvs
		double	m_dReqW;			//�ʿ� ���������� ��
		double	m_dUseT;			//��� ���򺸰��� �β� tvs		
		double	m_dReqT;			//�ʿ� ���򺸰��� �ּҵβ�
		double	m_dUseI;			//��� ����..
		double	m_dReqI;			//�ʿ� ����..
		CString m_sReqPlace;		//��ġ��ġ�� ������..������ġ
		CString m_sUsePlase;		//       //        ..��ġ��ġ
		double	m_dFlangeGap;		//���Ͼ��÷����� ������ b
		
	} m_strtHorStiff;

private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
};

#endif // !defined(AFX_DESIGNJIJUMSTIFF_H__DF7A5263_55A4_4A08_9989_CE35D78938C4__INCLUDED_)

// ������ ������
#if !defined(AFX_DESIGN_JIJUM_STIFF)
#define AFX_DESIGN_JIJUM_STIFF

class AFX_EXT_CLASS CDesignJijumStiff : public CObject
{
public:
	CDesignJijumStiff(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx, long nJoint);
	virtual ~CDesignJijumStiff();

	void SetJijumStiff();
	void SetJackupStiff();
	long GetJackupBuildsuByOneShoe(CPlateBasicIndex *pBx);


	struct _JijumStiff
	{
		CString	m_strMaterial;		//������ ����
		double	m_dReaction;		//�����ݷ�
		double  m_dWidth;			//��
		double  m_dLength;			//����
		double  m_dGap;				//��ġ����
		double	m_dUseT;			//�β�
		double  m_dReqT;			//�ʿ� �ּ� �β�
		double  m_dUse_EA;			//��� ����
		double  m_dWebT;			//���� �β�
		double  m_dDanW_Be;			//�ܸ���ȿ��
		double  m_dDanArea_As;		//�ܸ���
		double  m_dReqArea_Ae;		//��ȿ�ܸ��
		double  m_dMom_I;			//�ܸ�2�����Ʈ
		double  m_dDan_r;			//�ܸ� 2�� �ݰ�
		double  m_dBuckLing_le;		//��ȿ�±�����
		double  m_dFcag;			//�����±��� ������� ���� �������� ��������
		double  m_dFcal;			//�����±��� ���� �������
		double  m_dFcao;			//�����±��� ������� ���� �ִ� �������� ��������
		double  m_dFca;				//�������� ��������
		double  m_dFc;				//					
	} m_strJijumStiff;

	struct _JackupStiff
	{
		CString	m_strMaterial;		//��������� ����
		double	m_dReaction;		
		double  m_dWidthL;
		double  m_dWidthR;
		double  m_dLength;			//����
		double  m_dGap;				//��ġ����
		double	m_dUseT;			//�β�
		double  m_dReqT;			//�ʿ� �ּ� �β�
		double  m_dUse_EA;			//��� ����
		double  m_dQtyOneShoe;		//SHOE 1���Ҵ� JACK UP��ġ ��ġ����
		double  m_dWebT;			//���� �β�
		double  m_dDanW_Be;			//�ܸ���ȿ��
		double  m_dDanArea_As;		//�ܸ���
		double  m_dReqArea_Ae;		//��ȿ�ܸ��
		double  m_dMom_I;			//�ܸ�2�����Ʈ
		double  m_dDan_r;			//�ܸ� 2�� �ݰ�
		double  m_dBuckLing_le;		//��ȿ�±�����
		double  m_dFcag;			//�����±��� ������� ���� �������� ��������
		double  m_dFcal;			//�����±��� ���� �������
		double  m_dFcao;			//�����±��� ������� ���� �ִ� �������� ��������
		double  m_dFca;				//�������� ��������
		double  m_dFc;				//	
		double  m_dApplyLoad_P;		//���� ����...
		double  m_dPos_Yo;			//�߸����� ��ġ

	} m_strJackupStiff;

private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
	CPlateBasicIndex	*m_pBx;
	long				 m_nJoint;
};

#endif // !defined(AFX_DESIGN_JIJUM_STIFF)
