// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DesignCrossBeam.h: interface for the CDesignCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESIGNCROSSBEAM_H__40277C2E_5985_46F5_94F0_F07D1DD06723__INCLUDED_)
#define AFX_DESIGNCROSSBEAM_H__40277C2E_5985_46F5_94F0_F07D1DD06723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDesignEndCrossBeam;
class CDesignCenterCrossBeam;
class CFemStressCalc;
class CDesignInverseVBracing;
class CDesignVBracing;
class CDesignFlangeJoint;
class CDesignWebJoint;

#define	COMP_MOMENT		0
#define	COMP_SHEAR		1
#define COMP_HORIZONTAL	2
#define COMP_FLANGEUP	3
#define COMP_FLANGELO	4
#define COMP_ALLOWSHEAR	5

#define CROSSBEAMJOINT_GUSSETTYPE		0		// �ż�ƮŸ��
#define CROSSBEAMJOINT_PLATETYPE		1		// ����Ÿ��
#define CROSSBEAMWEB_USEJOINTTYPE		2		// ������ ������ Ÿ�Ի��
#define CROSSBEAMWEB_NOTJOINTTYPE		3		// ������ ������ ����


class AFX_EXT_CLASS CDesignCrossBeam : public CObject
{
public:
	CDesignCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignCrossBeam();

	void*	GetDesignCrossBeam();
	double	GetFlangeMoment();
	double	GetFlangeStress(BOOL bUpper);
	double	GetFlangeShear();
	double	GetBoltSize(CString strBoltSize);
private:
	CAPlateCalcStd			*m_pAPlateCalcStd;
	CPlateBasicIndex		*m_pBx;
	CDesignEndCrossBeam		*m_pEndCr;
	CDesignCenterCrossBeam	*m_pCenCr;
	CDesignInverseVBracing	*m_pInvVBracing;
	CDesignVBracing			*m_pVBracing;
};

#endif // !defined(AFX_DESIGNCROSSBEAM_H__40277C2E_5985_46F5_94F0_F07D1DD06723__INCLUDED_)


//////////////////////////////////////////////////////////////////////////
// ����Ÿ��.H����Ÿ�� ���� �� �β� ����
#if !defined(AFX_CROSSBEAM_JEWON)
#define AFX_CROSSBEAM_JEWON
class AFX_EXT_CLASS CCrossBeamJewon : public CObject
{
public:
	CCrossBeamJewon(long nMaterial);
	virtual ~CCrossBeamJewon();

	CString	m_strMaterial;
	long	m_nMaterial;
	double	m_dWidth;
	double	m_dHeight;
	double	m_dA;			// �ܸ���
	double	m_dY;			// ����
	double	m_dAY;
	double	m_dAY2;
	double	m_dIo;			// �ܸ�2�����Ʈ
};
#endif // !defined(AFX_CROSSBEAM_JEWON)

//////////////////////////////////////////////////////////////////////////
// �극�̽� ���� �� �β� ����
#if !defined(AFX_BRACING_JEWON)
#define AFX_BRACING_JEWON
class AFX_EXT_CLASS CBracingJewon : public CObject
{
public:
	CBracingJewon();
	virtual ~CBracingJewon();

	long	m_nType;
	double	m_dWidth;
	double	m_dHeight;
	double	m_dThick1;
	double	m_dThick2;
	double	m_dArea;
	double	m_dGammaX;
	double	m_dGammaY;
	double	m_dGammaMin;
	double	m_dLength;
	double	m_dZy;
};
#endif // !defined(AFX_BRACING_JEWON)

//////////////////////////////////////////////////////////////////////////
// Frame �ؼ����
#if !defined(AFX_CROSSBEAM_FRAME_ELEMENTLOAD)
#define AFX_CROSSBEAM_FRAME_ELEMENTLOAD
class AFX_EXT_CLASS CFrameElementLoad : public CObject
{
public:
	CFrameElementLoad();
	virtual ~CFrameElementLoad();

	void SetFrameLoad(CFemStressCalc *pFemStressCalc, long nElement);

	struct _MOMENT
	{
		double	m_dPreDeadLoad;
		double	m_dPostDeadLoad;
		double	m_dMaxLiveLoad;
		double	m_dMinLiveLoad;
		double	m_dMaxSettlement;
		double	m_dMinSettlement;
	} m_strtMoment;
	struct _SHEAR
	{
		double	m_dPreDeadLoad;
		double	m_dPostDeadLoad;
		double	m_dMaxLiveLoad;
		double	m_dMinLiveLoad;
		double	m_dMaxSettlement;
		double	m_dMinSettlement;
	} m_strtShear;
	struct _AXIAL
	{
		double	m_dPreDeadLoad;
		double	m_dPostDeadLoad;
		double	m_dMaxLiveLoad;
		double	m_dMinLiveLoad;
		double	m_dMaxSettlement;
		double	m_dMinSettlement;
	} m_strtAxial;
	struct _TOTALLOAD
	{
		double	m_dMaxMoment;
		double	m_dMinMoment;
		double	m_dMaxShear;
		double	m_dMinShear;
		double	m_dMaxAxial;
		double	m_dMinAxial;
	} m_strtTotLoad;
private:
	long	m_nElement;
};
#endif // !defined(AFX_CROSSBEAM_FRAME_ELEMENTLOAD)

//////////////////////////////////////////////////////////////////////////
// ����������
#if !defined(AFX_CROSSBEAM_VSTIFF)
#define AFX_CROSSBEAM_VSTIFF
class AFX_EXT_CLASS CCrossBeamVerStiff : public CObject
{
public:
	CCrossBeamVerStiff(CPlateBasicIndex *pBx, CAPlateCalcStd *pAPlateCalcStd);
	virtual ~CCrossBeamVerStiff();

	void SetCrossBeamVerStiff();
	CString	m_strMaterial;
	CString	m_strRight;
	BOOL	m_bIsNeedVerStiff;		// ���������� �ʿ� ����
	double	m_dMaxSpace;			// ���������縦 ������ �� �ִ� �÷��� �������� �ִ�ġ
	double	m_dMaxSpaceCoeff;
	double	m_dVerStiffSpace;		// ������������ ����
	double	m_dSpaceCoeff;
	double	m_dSpaceCoeffUse;
	double	m_dBvs;					// ������������ ��
	double	m_dTvs;					// ������������ �β�
	double	m_dAllowShear;			// �����������
	double	m_dShear;				// ������ ��������
	double	m_dHeightWeb;			// ���� ����
	double	m_dThickWeb;			// ���� �β�
	double	m_dSigma;				// ������ ��������
	double	m_dBvsMin;
	double	m_dTvsMin;
	double	m_dRateSpaceHeight;
	double	m_dAllowSpaceCoeff;
	long	m_nQtyHorStiff;
	double	m_dGamma;
	double	m_dIReq;
	double	m_dIUse;
	long	m_nVerSide;
private:
	CPlateBasicIndex	*m_pBx;
	CAPlateCalcStd		*m_pAPlateCalcStd;
};
#endif // !defined(AFX_CROSSBEAM_VSTIFF)

//////////////////////////////////////////////////////////////////////////
// ���򺸰���
#if !defined(AFX_CROSSBEAM_HSTIFF)
#define AFX_CROSSBEAM_HSTIFF
class AFX_EXT_CLASS CCrossBeamHorStiff : public CObject
{
public:
	CCrossBeamHorStiff(CPlateBasicIndex *pBx);
	virtual ~CCrossBeamHorStiff();

	void SetCrossBeamHorStiff();

	CString	m_strMaterial;
	double	m_dHeightWeb;
	double	m_dThickWeb;
	double	m_dVerStiffSpace;		// ������������ ����
	double	m_dBhs;					// ���򺸰����� ��
	double	m_dThs;					// ���򺸰����� �β�
	double	m_dThsMin;
	double	m_dGamma;
	double	m_dIReq;
	double	m_dIUse;
	long	m_nHorSide;
private:
	CPlateBasicIndex	*m_pBx;
};
#endif // !defined(AFX_CROSSBEAM_HSTIFF)

//////////////////////////////////////////////////////////////////////////
// �ܺ� ���κ�(����Ÿ��.H����Ÿ��)
#if !defined(AFX_DESIGN_ENDCROSSBEAM)
#define AFX_DESIGN_ENDCROSSBEAM

class AFX_EXT_CLASS CDesignEndCrossBeam : public CObject
{
public:
	CDesignEndCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignEndCrossBeam();

	void SetDataInit();
	void SetGeneralType();

//-> �Ϲ�Ÿ��
	void SetJewon();				// ��������
	void SetVerLoad();				// ��������
	void SetHorLoad();				// ��������
	void SetEarthQuakeLoad();		// ��������
	void SetElementLoad();			// ����� ���
	void SetFrameElementLoad();		// Frame �ؼ����
	void SetDesignSection();		// �ܸ鼳��
	void SetCompositeStress();		// ������� ���չ� ���°���
	void SetMinThick();

	long	m_nQtyHorStiff;						// ���򺸰��� �ܼ�

	struct _MINTHICK							// �ּҵβ�
	{
		double	m_dThickWebReq;					// ���� �ʿ�β�
		double	m_dMinThickFlangeUp;			// ��� �÷��� �ּҵβ�
		double	m_dMinThickFlangeLo;			// �Ϻ� �÷��� �ּҵβ�
	} m_strtMinThick;

	struct _VERLOAD								// ��������
	{
		// ��������
		double	m_dGirderSpace;					// ��������
		double	m_dDistVerBracingToEndSlab;		// �����극�̽̿��� ���Ǵܺα��� �Ÿ�
		double	m_dLoadPave;					// ��������
		double	m_dLoadSlab;					// ��������
		double	m_dQ1;
		double	m_dQ2;
		// Ȱ����
		double	m_dCollisionCoeff;				// ��ݰ��(���)
		double	m_dCollisionCoeffUse;			// ��ݰ��(���)
	} m_strtVerLoad;

	struct _HORLOAD								// ��������
	{
		double	m_dWidthBridge;					// ��������
		double	m_dHeightBridge;				// ��������
		double	m_dRateWidthHeight;
		double	m_dWw;							// ǳ���߰���(���)
		double	m_dWwUse;						// ǳ���߰���(���)
		double	m_dLengthSpan;					// �ش� ��������
		double	m_dHw;							// ǳ���߿� ���� �극�̽� 1���� �ۿ��
	} m_strtHorLoad;

	struct _EARTHQUAKELOAD						// ��������
	{
		double	m_dReaction;					// �ش������ݷ� �հ�
		double	m_dAcceleration;				// �������ӵ� ���
		double	m_dTotEarthQuakeLoad;			// �ܺ� �극�̽̿� �ۿ��ϴ� ����������
		double	m_dEarthQuakeLoad;				// �������߿� ���� �극�̽� 1���� �ۿ��
	} m_strtEarthQuakeLoad;

	struct _ELEMENTLOAD
	{
		double	m_dMd;							// �������߿� ���� �ڸ��Ʈ
		double	m_dMl;							// Ȱ���߿� ���� �ڸ��Ʈ
		double	m_dSd;							// �������߿� ���� ���ܷ�
		double	m_dSl;							// Ȱ���߿� ���� ���ܷ�
		double	m_dHw;							// ǳ���߿� ���� �����
		double	m_dHe;							// �������� ���� �����
	} m_strtElementLoad;

	struct _DESIGNSECTION
	{
		double	m_dDelta;
		double	m_dI;
		double	m_dYsu;
		double	m_dYsl;
		double	m_dSigmaca;
		double	m_dFca;
		double	m_dSigmata;
		double	m_dSheara;
	} m_strtSection;

	struct _FRAMELOADCOMPOSITE					// ����� ���չ� ���°���
	{
		double	m_dDeadLoad[6];
		double	m_dLiveLoad[6];
		double	m_dWindLoad[6];
		double	m_dEarthQuakeLoad[6];
		double	m_dCompGen[6];
		double	m_dCompWind[6];
		double	m_dCompEarthQuake[6];
		double	m_dCompFrameLoad[6];
		double	m_dMaxMoment;
		double	m_dMaxShear;
		double	m_dMaxFlangeUp;
		double	m_dMaxFlangeLo;
	} m_strtFrameLoadComp;

	long	GetHorStiff()		{ return m_nQtyHorStiff;														};
	double	GetTotSectionArea()	{ return m_pJewonFlangeUp->m_dA+m_pJewonFlangeLo->m_dA+m_pJewonWeb->m_dA;		};
	double	GetTotSectionAY()	{ return m_pJewonFlangeUp->m_dAY+m_pJewonFlangeLo->m_dAY+m_pJewonWeb->m_dAY;	};
	double	GetTotSectionAY2()	{ return m_pJewonFlangeUp->m_dAY2+m_pJewonFlangeLo->m_dAY2+m_pJewonWeb->m_dAY2;	};
	double	GetTotSectionIo()	{ return m_pJewonFlangeUp->m_dIo+m_pJewonFlangeLo->m_dIo+m_pJewonWeb->m_dIo;	};

	CCrossBeamJewon			*m_pJewonFlangeUp;
	CCrossBeamJewon			*m_pJewonFlangeLo;
	CCrossBeamJewon			*m_pJewonWeb;
	CFrameElementLoad		*m_pFrameElementLoad;
	CCrossBeamVerStiff		*m_pVerStiff;
	CCrossBeamHorStiff		*m_pHorStiff;
private:
	CAPlateCalcStd			*m_pAPlateCalcStd;
	CPlateBasicIndex		*m_pBx;
};
#endif // !defined(AFX_DESIGN_ENDCROSSBEAM)

//////////////////////////////////////////////////////////////////////////
// �߰� ���κ�(����Ÿ��.H����Ÿ��)
#if !defined(AFX_DESIGN_CENTERCROSSBEAM)
#define AFX_DESIGN_CENTERCROSSBEAM
class AFX_EXT_CLASS CDesignCenterCrossBeam : public CObject
{
public:
	CDesignCenterCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignCenterCrossBeam();
	void SetDataInit();
	void SetGeneralType();

	void SetJewon();				// ��������
	void SetFrameElementLoad();		// Frame �ؼ����
	void SetDesignSection();		// �ܸ鼳��
	void SetMinThick();

	struct _DESIGNSECTION
	{
		double	m_dDelta;
		double	m_dI;
		double	m_dYsu;
		double	m_dYsl;
		double	m_dSigmaca;
		double	m_dFca;
		double	m_dSigmata;
		double	m_dSheara;
		double	m_dFu;
		double	m_dFl;
		double	m_dShear;
		double	m_dCompStress;
	} m_strtSection;

	struct _MINTHICK							// �ּҵβ�
	{
		double	m_dThickWebReq;					// ���� �ʿ�β�
		double	m_dMinThickFlangeUp;			// ��� �÷��� �ּҵβ�
		double	m_dMinThickFlangeLo;			// �Ϻ� �÷��� �ּҵβ�
	} m_strtMinThick;

	double	m_dGirderSpace;
	long	m_nQtyHorStiff;
	long	GetHorStiff()		{ return m_nQtyHorStiff;														};
	double	GetTotSectionArea()	{ return m_pJewonFlangeUp->m_dA+m_pJewonFlangeLo->m_dA+m_pJewonWeb->m_dA;		};
	double	GetTotSectionAY()	{ return m_pJewonFlangeUp->m_dAY+m_pJewonFlangeLo->m_dAY+m_pJewonWeb->m_dAY;	};
	double	GetTotSectionAY2()	{ return m_pJewonFlangeUp->m_dAY2+m_pJewonFlangeLo->m_dAY2+m_pJewonWeb->m_dAY2;	};
	double	GetTotSectionIo()	{ return m_pJewonFlangeUp->m_dIo+m_pJewonFlangeLo->m_dIo+m_pJewonWeb->m_dIo;	};

	CCrossBeamJewon			*m_pJewonFlangeUp;
	CCrossBeamJewon			*m_pJewonFlangeLo;
	CCrossBeamJewon			*m_pJewonWeb;
	CFrameElementLoad		*m_pFrameElementLoad;
	CCrossBeamVerStiff		*m_pVerStiff;
	CCrossBeamHorStiff		*m_pHorStiff;
private:
	CAPlateCalcStd			*m_pAPlateCalcStd;
	CPlateBasicIndex		*m_pBx;
};
#endif // !defined(AFX_DESIGN_CENTERCROSSBEAM)

//////////////////////////////////////////////////////////////////////////
// �ܺ� �����극�̽�(��V�� Ÿ��)
#if !defined(AFX_DESIGN_INVERVBRACING)
#define AFX_DESIGN_INVERVBRACING

class AFX_EXT_CLASS CDesignInverseVBracing : public CObject
{
public:
	CDesignInverseVBracing(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignInverseVBracing();
	void SetDataInit();
	void SetInverseVBracingType();
//-> �ܺ� �����극�̽�(��V��)
	void SetJewon();			// ��밭��
	void SetVerLoad();			// ��������
	void SetHorLoad();			// ��������
	void SetEarthQuakeLoad();	// ��������-��������
	void SetElementLoad();		// ����� ��� - ������.����.������
	void SetDesignElement();	// ������ ���� - ������.����.������
	void SetGussetPlate();		// Gusset Plate ���� - ������.����.������
	double	m_dGirderSpace;
	double	m_dHeight;
	double	m_dTheta;
	struct _VERLOAD								// ��������
	{
		// ��������
		double	m_dDistVerBracingToEndSlab;		// �����극�̽̿��� ���Ǵܺα��� �Ÿ�
		double	m_dLoadPave;					// ��������
		double	m_dLoadSlab;					// ��������
		double	m_dQ1;
		double	m_dQ2;
		double	m_dPd;
		// Ȱ����
		double	m_dLength;						// Ȱ���߿� ���� ����
		double	m_dN1;
		double	m_dN2;
		double	m_dN3;
	} m_strtVerLoad;
	struct _HORLOAD								// ��������
	{
		double	m_dWidthBridge;					// ��������
		double	m_dHeightBridge;				// ��������
		double	m_dRateWidthHeight;
		double	m_dWw;							// ǳ���߰���(���)
		double	m_dWwUse;						// ǳ���߰���(���)
		double	m_dLengthSpan;					// �ش� ��������
		double	m_dHw;							// ǳ���߿� ���� �극�̽� 1���� �ۿ��
		double	m_dReaction;					// �ش������ݷ� �հ�
		double	m_dAcceleration;				// �������ӵ� ���
		double	m_dTotEarthQuakeLoad;			// �ܺ� �극�̽̿� �ۿ��ϴ� ����������
		double	m_dEarthQuakeLoad;				// �������߿� ���� �극�̽� 1���� �ۿ��
		double	m_dH;
	} m_strtHorLoad;

	struct _ELEMENTLOAD
	{
		double	m_dCollCoefUp;					// ������ ��ݰ��
		double	m_dCollCoefLo;					// ������ ��ݰ��
		double	m_dCollCoefDiagonal;			// ���� ��ݰ��
		double	m_dN1;							// ������ �����������Ͻ�
		double	m_dMl;							// ������ ���� Ȱ���� ���Ͻ�
		double	m_dN2Hor;						// ���� (�������߸� �ۿ��, BD���� ����� �ִ�)
		double	m_dN2VerHor;					// ���� (����+�������� �ۿ��, BE���� ����� �ִ�)
		double	m_dN3Ver;						// ������ (�������߸� �ۿ��, ����� �ִ�)
		double	m_dN3Hor;						// ������ (�������߸� �ۿ��, ����� �ִ�)
	} m_strtElementLoad;

	struct _DESIGN_CHORDUP						// ������
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// ��������..CT�����̳� �ƴϳ�...
		double	m_dSlendernessRatio;			// �����
		double	m_dFca;							// ��� ����� ��������
		double	m_dFcaApo;						// ����������� �����������
		double	m_dFa;
		double	m_dFc;
		double	m_dF;
		double	m_dAllowShear;					// Fillet �������� �������
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dN1Apo;						// 
		double	m_dShear;
	} m_strtChordUp;

	struct _DESIGN_DIAGONAL						// ����
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// ��������..CT�����̳� �ƴϳ�...
		double	m_dSlendernessRatio;			// �����
		double	m_dFca;							// ��� ����� ��������
		double	m_dFcaApo;						// ����������� �����������
		double	m_dFta;							// �����������
		double	m_dAn;							// ��ȿ�ܸ���
		double	m_dFt;							//
		double	m_dFc;
		double	m_dAllowShear;					// Fillet �������� �������
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dN2Apo;						// 
		double	m_dShear;
	} m_strtDiagonal;

	struct _DESIGN_CHORDLO						// ������
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// ��������..CT�����̳� �ƴϳ�...
		double	m_dSlendernessRatio;			// �����
		double	m_dFca;							// ��� ����� ��������
		double	m_dFcaApo;						// ����������� �����������
		double	m_dFta;							// �����������
		double	m_dAn;							// ��ȿ�ܸ���
		double	m_dFt;							//
		double	m_dFc;
		double	m_dAllowShear;					// Fillet �������� �������
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dN3Apo;						// 
		double	m_dShear;
	} m_strtChordLo;

	struct _GUSSETUP							// ������ ����
	{
		CString	m_strMaterial;
		double	m_dHeight;						// ����
		double	m_dThick;						// �β�
		double	m_dWidth;						// ����
		double	m_dAllowStress;					// ������ �������
		double	m_dLoad;						// �ۿ�����
		CString	m_strBoltMaterial;				// ��Ʈ
		double	m_dDiaBolt;						// ��Ʈ ����
		double	m_dBoltAllowStress;				// ��Ʈ �������
		double	m_dQtyBoltReq;					// �ʿ亼Ʈ��
		double	m_dQtyBoltUse;					// ��뺼Ʈ��
		double	m_dAn;							// Gusset ���ܸ���
		double	m_dStress;						// 
	} m_strtGussetUp;

	struct _GUSSETDIAGONAL						// ����/������ ����
	{
		CString	m_strMaterial;
		double	m_dHeight;						// ����
		double	m_dThick;						// �β�
		double	m_dWidth;						// ����
		double	m_dAllowStress;					// ������ �������
		double	m_dAllowShear;					// ������ �����������
		double	m_dLoadHor;						// �ۿ�����
		double	m_dLoadVer;
		double	m_dLoad;
		double	m_dAllowShearWeld;
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dShearWeld;					// ������ ���ܷ�
		double	m_dAgv;
		double	m_dAgh;
		double	m_dStress;
		double	m_dShearGusset;
		double	m_dCompStress;
	} m_strtGussetDiagonal;

	struct _GUSSETLO							// ������/���� ����
	{
		CString	m_strMaterial;
		double	m_dHeight;						// ����
		double	m_dThick;						// �β�
		double	m_dWidth;						// ����
		double	m_dAllowStress;					// ������ �������
		double	m_dRh;
		double	m_dRv;
		double	m_dLoad;						// �ۿ�����
		CString	m_strBoltMaterial;				// ��Ʈ
		double	m_dDiaBolt;						// ��Ʈ ����
		double	m_dBoltAllowStress;				// ��Ʈ �������
		double	m_dQtyBoltReq;					// �ʿ亼Ʈ��
		double	m_dQtyBoltUse;					// ��뺼Ʈ��
		double	m_dAn;							// Gusset ���ܸ���
		double	m_dStress;						// 
	} m_strtGussetLo;

	CBracingJewon			*m_pChordUp;
	CBracingJewon			*m_pChordLo;
	CBracingJewon			*m_pDiagonal;
private:
	CAPlateCalcStd			*m_pAPlateCalcStd;
	CPlateBasicIndex		*m_pBx;
};
#endif // !defined(AFX_DESIGN_INVERVBRACING)

//////////////////////////////////////////////////////////////////////////
// �߰� �����극�̽�(V�� Ÿ��)
#if !defined(AFX_DESIGN_VBRACING)
#define AFX_DESIGN_VBRACING

class AFX_EXT_CLASS CDesignVBracing : public CObject
{
public:
	CDesignVBracing(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignVBracing();
	void SetVBracingType();
	void SetDataInit();
	void SetJewon();
	void SetLoadCalculate();
	void SetElementLoad();
	void SetDesignElement();
	void SetGussetPlate();

	double GetVBracingSpace();

	//-> �߰� �����극�̽�(V�� Ÿ��)
	struct _WINDLOAD
	{
		double	m_dWidthBridge;					// ��������
		double	m_dHeightBridge;				// ��������
		double	m_dRateWidthHeight;
		double	m_dWw;							// ǳ������ ����
		double	m_dWwUse;						//
		double	m_dBracingSpace;				// �극�̽� ����
		double	m_dAllowCoeff;					// ǳ���� ���� ������� �������
		double	m_dHw;							// ǳ���߿� ���� �극�̽� 1���� �ۿ��
	} m_strtWind;

	struct _EARTHQUAKE
	{
		double	m_dReaction;					// �ݷ��� ���հ�
		double	m_dLengthBridge;				// ��������
		double	m_dWd;							// �������̴� ��������
		double	m_dAcceleration;				// �������ӵ� ���
		double	m_dWe;							// �������̴� ��������
		double	m_dHe;							// �������߿� ���� �극�̽� 1���� �ۿ��
		double	m_dAllowCoeff;					// �������� ���� ������� �������
		double	m_dH;
	} m_strtEarthQuake;

	struct _ELEMENTLOAD
	{
		double	m_dN1;							// ������(AB) - ����
		double	m_dN2Tension;					// ����((AD:����)
		double	m_dN2Comp;						// ����((BD:����)
		double	m_dN3;							// ������(CD) - ����
	} m_strtElementLoad;

	struct _DESIGN_CHORDUP						// ������
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// ��������..CT�����̳� �ƴϳ�...
		double	m_dSlendernessRatio;			// �����
		double	m_dFca;							// ��� ����� ��������
		double	m_dFcaApo;						// ����������� �����������
		double	m_dFc;
		double	m_dAllowShear;					// Fillet �������� �������
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dN1Apo;						// 
		double	m_dShear;
	} m_strtChordUp;

	struct _DESIGN_DIAGONAL						// ����
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// ��������..CT�����̳� �ƴϳ�...
		double	m_dSlendernessRatio;			// �����
		double	m_dFca;							// ��� ����� ��������
		double	m_dFcaApo;						// ����������� �����������
		double	m_dFta;							// �����������
		double	m_dAn;							// ��ȿ�ܸ���
		double	m_dFt;							//
		double	m_dFc;
		double	m_dAllowShear;					// Fillet �������� �������
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dN2Apo;						// 
		double	m_dShear;
	} m_strtDiagonal;

	struct _DESIGN_CHORDLO						// ������
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// ��������..CT�����̳� �ƴϳ�...
		double	m_dSlendernessRatio;			// �����
		double	m_dFca;							// ��� ����� ��������
		double	m_dFcaApo;						// ����������� �����������
		double	m_dAn;							// ��ȿ�ܸ���
		double	m_dFc;
		double	m_dAllowShear;					// Fillet �������� �������
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dN3Apo;						// 
		double	m_dShear;
	} m_strtChordLo;

	struct _GUSSETUP							// ������/���� ����
	{
		CString	m_strMaterial;
		double	m_dHeight;						// ����
		double	m_dThick;						// �β�
		double	m_dWidth;						// ����
		double	m_dAllowStress;					// ������ �������
		double	m_dRh;
		double	m_dRv;
		double	m_dLoad;						// �ۿ�����
		CString	m_strBoltMaterial;				// ��Ʈ
		double	m_dDiaBolt;						// ��Ʈ ����
		double	m_dBoltAllowStress;				// ��Ʈ �������
		double	m_dQtyBoltReq;					// �ʿ亼Ʈ��
		double	m_dQtyBoltUse;					// ��뺼Ʈ��
		double	m_dAn;							// Gusset ���ܸ���
		double	m_dStress;						// 
	} m_strtGussetUp;

	struct _GUSSETLO							// ������ ����
	{
		CString	m_strMaterial;
		double	m_dHeight;						// ����
		double	m_dThick;						// �β�
		double	m_dWidth;						// ����
		double	m_dAllowStress;					// ������ �������
		double	m_dLoad;						// �ۿ�����
		CString	m_strBoltMaterial;				// ��Ʈ
		double	m_dDiaBolt;						// ��Ʈ ����
		double	m_dBoltAllowStress;				// ��Ʈ �������
		double	m_dQtyBoltReq;					// �ʿ亼Ʈ��
		double	m_dQtyBoltUse;					// ��뺼Ʈ��
		double	m_dAn;							// Gusset ���ܸ���
		double	m_dStress;						// 
	} m_strtGussetLo;

	struct _GUSSETDIAGONAL						// ����/������ ����
	{
		CString	m_strMaterial;
		double	m_dHeight;						// ����
		double	m_dThick;						// �β�
		double	m_dWidth;						// ����
		double	m_dAllowStress;					// ������ �������
		double	m_dAllowShear;					// ������ �����������
		double	m_dLoad;
		double	m_dAllowShearWeld;
		double	m_dLengthWeld;					// Fillet ��������
		double	m_dThroatDepth;					// Fillet ������β�
		double	m_dShearWeld;					// ������ ���ܷ�
		double	m_dAg;
		double	m_dShear;
	} m_strtGussetDiagonal;
	
	double	m_dGirderSpace;
	double	m_dHeight;
	double	m_dTheta;
	CBracingJewon			*m_pChordUp;
	CBracingJewon			*m_pChordLo;
	CBracingJewon			*m_pDiagonal;
private:
	CAPlateCalcStd			*m_pAPlateCalcStd;
	CPlateBasicIndex		*m_pBx;
};
#endif // !defined(AFX_DESIGN_VBRACING)

//////////////////////////////////////////////////////////////////////////
// ���κ� ����
#if !defined(AFX_DESIGN_CROSSBEAM_JOINT)
#define AFX_DESIGN_CROSSBEAM_JOINT

class AFX_EXT_CLASS CDesignCrossBeamJoint : public CObject
{
public:
	CDesignCrossBeamJoint(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignCrossBeamJoint();

	void SetCrossBeamJoint();

	CDesignFlangeJoint	*m_pUpper;
	CDesignFlangeJoint	*m_pLower;	

	CDesignWebJoint		*m_pWebLeft;
	CDesignWebJoint		*m_pWebRight;

private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
	CPlateBasicIndex	*m_pBx;
};
#endif // !defined(AFX_DESIGN_CROSSBEAM_JOINT)

//////////////////////////////////////////////////////////////////////////
// ���κ� �÷��� ����
#if !defined(AFX_DESIGN_CROSSBEAM_FLANGE_JOINT)
#define AFX_DESIGN_CROSSBEAM_FLANGE_JOINT

class AFX_EXT_CLASS CDesignFlangeJoint : public CObject
{
public:
	CDesignFlangeJoint(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignFlangeJoint();

	void SetFlangeJoint();
	void SetGussetType();
	void SetPlateType();

	struct _GUESSET								// �ż�Ʈ���
	{
		CString	m_strMaterial;
		CString m_sRightMaterial;				// ���� ������ �̸�
		double	m_dStress;						// �ۿ�����
		double	m_dAllowStress;					// �������
		double  m_dDesignStress;					// ��������
		double	m_dAxialLoad;					// �������
		double	m_dAreaFlange;					// �÷��� �ܸ���
		CString	m_strBoltMaterial;				// ��Ʈ
		CString m_sLeftBoltMaterial;			//  F8T ���� F8��.
		double	m_dDiaBolt;						// ��Ʈ����
		double	m_dAllowBolt;					// ��Ʈ 1���� �������
		long	m_nQtySide;						// 1�鸶��, 2�鸶��
		double	m_dBoltReq;						// �ʿ亼Ʈ��
		double	m_dBoltUse;						// ��뺼Ʈ��
		long	m_nQtyLine;						// ��뿭��
		long	m_nTopConEA;					// ��� ������ ����
		long	m_nBottomConEA;					// �ϸ� ������ ����
		double  m_dTopWidth;					// ��� PL��
		double  m_dBottomWidth;					// �ϸ� PL��
		double  m_dTopThick;					// ��� PL�β�
		double  m_dBottomThick;					// �ϸ� PL�β�
		double  m_dTR;							// �ż��� �߽� ����		
		double	m_dSumAs;						// ���ܸ���
		double	m_dAsn;							// ���ܸ���
		double  m_dConStress;					// ������ ����		
						
		double  m_dWidth;
		double  m_dThick;				
	} m_strtGusset;

	struct _PLATE
	{
		CString	m_strMaterial;
		double	m_dStress;						// �ۿ�����
		double	m_dAllowStress;					// �������
		double	m_dDesignStress;				// ��������
		double	m_dThick;
		double	m_dWidth;
		double	m_dAf;							// ����ܸ���
		CString	m_strBoltMaterial;				// ��Ʈ ����
		CString m_sLeftBoltMaterial;			//  F8T ���� F8��.
		double	m_dDiaBolt;						// ��Ʈ ����
		double	m_dAllowBolt;					// ��Ʈ 1���� �������
		double	m_dQtyBoltReq;					// �ʿ亼Ʈ��
		double	m_dQtyBoltUse;					// ��뺼Ʈ��
		long	m_nQtyLine;						// ��ġ����
		double	m_dTw;
		double	m_dL1;
		double	m_dA1;
		double	m_dA2;
		double	m_dA3;
		double	m_dTsMin;
		double	m_dTsUse;
		double	m_dTsCoeff;
		double	m_dSumAs;
		double	m_dAsn;							// �Ϻ� ���ܸ���
		double	m_dShear;						// �ۿ���������
		double  m_dQ1;
		double	m_dQ;							// �߸��࿡���� ���ռ������� �ܸ�1�����Ʈ		
		double	m_dI;							// �߸��࿡���� �Ѵܸ��� �ܸ�2�����Ʈ
		double	m_dRhop;						// ������
		double	m_dRhoh;						// ����������
		double	m_dRho;							// �ռ����� ����
		double	m_dAreaFlange;					// �÷��� �Ѵܸ���
		double	m_dAsnFlange;					// ��Ʈ������ ������ �÷��� ���ܸ���
		double	m_dStressFlange;						// ������ ����
		long	m_nTopConEA;					// ��� ������ ����
		long	m_nBottomConEA;					// �ϸ� ������ ����
	} m_strtPlate;

	long	m_nType;							// �ż�Ʈ���(��ü��,�и���), �������
	BOOL	m_bIsUpper;							// ���Ϻ� �÷���
private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
	CPlateBasicIndex	*m_pBx;
};
#endif // !defined(AFX_DESIGN_CROSSBEAM_FLANGE_JOINT)

//////////////////////////////////////////////////////////////////////////
// ���κ� ���� ����
#if !defined(AFX_DESIGN_CROSSBEAM_WEB_JOINT)
#define AFX_DESIGN_CROSSBEAM_WEB_JOINT

class AFX_EXT_CLASS CDesignWebJoint : public CObject
{
public:
	CDesignWebJoint(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignWebJoint();

	void SetWebJoint();
	void SetGussetType();
	void SetPlateType();

	struct _GUESSET								// �ż�Ʈ���
	{
		CString	m_strMaterial;
		double  m_dApplyLoad;		
		CString	m_strBoltMaterial;				// ��Ʈ
		CString m_sLeftBoltMaterial;			//  F8T ���� F8��.
		double	m_dDiaBolt;						// ��Ʈ����
		double	m_dAllowBolt;					// ��Ʈ 1���� �������
		double	m_dBoltReq;						// �ʿ亼Ʈ��
		double	m_dBoltUse;						// ��뺼Ʈ��
		long	m_nQtyLine;						// ��뿭��	

	} m_strtGusset;

	struct _PLATE
	{
		double	m_dTw;
		long	m_nB1;
		double	m_dB1;
		double	m_dB2;
		double	m_dB;
		double	m_dF1;
		double	m_dF2;
		double	m_dF3;
		double	m_dF4;
		double	m_dHo;
		double	m_dRhom;						// �ڸ��Ʈ�� ���� ����Ʈ�� �ۿ�� ( 2�鸶�� )
		double	m_dRhos;						// ���ܷ¿� ���� ����Ʈ�� �ۿ�� ( 2������ )
		double	m_dRho;							// �ռ������� ����
		double	m_dTsCoeff;
		double	m_dTsMin;
		double	m_dTsUse;
		double	m_dMoment;						// �ۿ� ���Ʈ
		double	m_dIw;							// ���θ��� �ܸ�2�� ���Ʈ
		double  m_dI;							// ���κ� ��ü�� �ܸ�2�����Ʈ
		double	m_dMw;							// ���ΰ� ����ϴ� �ڸ��Ʈ
		double	m_Is;							// SPLICE�� �ܸ�2�� ���Ʈ
		double	m_dFMax;						// SPLICE�� �ִ�����
		double  m_dReqArea;						// �ʿ�ܸ���..
		double	m_dSumAs;
		double	m_dShear;
		double  m_dAllowTension;
		double	m_dAllowShear;
		double	m_dCompStress;
		CString	m_strMaterial;					// ����
		double  m_dStress;						// �ۿ�����
		double  m_dA;
		double  m_dA2;
		double  m_dsE;
		double  m_dsF;
		double  m_dTop;
		double	m_dAllowBolt;					// ��Ʈ 1���� �������
		double	m_dDiaBolt;						// ��Ʈ ����
		double  m_dsT1;
		double  m_dsT2;
		double  m_dApplyLoad;					//�ۿ����ܷ�...
	} m_strtPlate;

	long	m_nType;							// �ż�Ʈ���, �������
private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
	CPlateBasicIndex	*m_pBx;
};
#endif // !defined(AFX_DESIGN_CROSSBEAM_WEB_JOINT)

//////////////////////////////////////////////////////////////////////////
// ����극�̽�
#if !defined(AFX_DESIGN_HOR_BRACING)
#define AFX_DESIGN_HOR_BRACING

class AFX_EXT_CLASS CDesignHorBracing : public CObject
{
public:
	CDesignHorBracing(CAPlateCalcStd *pAPlateCalcStd, long nG, long nIdxBracing);
	virtual ~CDesignHorBracing();

	void SetHorBracing();

	long	m_nG;
	long	m_nIndex;						// ����극�̽� Index
	double	m_dWindLoad;					// ǳ����
	double	m_dWw;							// ǳ���� ����
	double	m_dEarthQuakeLoad;				// ��������
	double	m_dWe;							// �������� ����
	double	m_dLengthSection;				// ������
	double	m_dAreaPositiveInf;				// ���⼱ ����(+)
	double	m_dAreaNegativeInf;				// ���⼱ ����(-)
	double	m_dTheta;
	double	m_dLengthElem;					// ������
	double	m_dNwPositive;
	double	m_dNwNegative;
	double	m_dNe;
	CString	m_strMaterial;
	CString m_strMaterialType;
	double	m_dSlendernessRatio;			// �����
	double	m_dFca;							// ��� ����� ��������
	double	m_dFcaApo;						// ����������� �����������
	double	m_dFta;
	double	m_dAn;
	double	m_dFt;
	double	m_dFc;
	double	m_dDesignLoad;					// �����ۿ��
	CString	m_strBoltMaterial;				// ��Ʈ ����
	double	m_dDiaBolt;						// ��Ʈ ����
	double	m_dBoltAllowStress;
	double	m_dQtyBoltReq;					// �ʿ亼Ʈ��
	double	m_dQtyBoltUse;					// ��뺼Ʈ��
	long	m_nQtyLine;						// ��ġ����
	
	CBracingJewon		*m_pBracingJewon;
private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
};
#endif // !defined(AFX_DESIGN_HOR_BRACING)