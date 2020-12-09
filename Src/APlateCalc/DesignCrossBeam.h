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

#define CROSSBEAMJOINT_GUSSETTYPE		0		// 거세트타입
#define CROSSBEAMJOINT_PLATETYPE		1		// 기존타입
#define CROSSBEAMWEB_USEJOINTTYPE		2		// 복부판 이음판 타입사용
#define CROSSBEAMWEB_NOTJOINTTYPE		3		// 복부판 이음판 없음


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
// 제작타입.H형강타입 제원 및 두께 검토
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
	double	m_dA;			// 단면적
	double	m_dY;			// 도심
	double	m_dAY;
	double	m_dAY2;
	double	m_dIo;			// 단면2차모멘트
};
#endif // !defined(AFX_CROSSBEAM_JEWON)

//////////////////////////////////////////////////////////////////////////
// 브레이싱 제원 및 두께 검토
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
// Frame 해석결과
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
// 수직보강재
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
	BOOL	m_bIsNeedVerStiff;		// 수직보강재 필요 여부
	double	m_dMaxSpace;			// 수직보강재를 생략할 수 있는 플랜지 순간격의 최대치
	double	m_dMaxSpaceCoeff;
	double	m_dVerStiffSpace;		// 수직보강재의 간격
	double	m_dSpaceCoeff;
	double	m_dSpaceCoeffUse;
	double	m_dBvs;					// 사용수직보강재 폭
	double	m_dTvs;					// 사용수직보강재 두께
	double	m_dAllowShear;			// 허용전단응력
	double	m_dShear;				// 복부판 전단응력
	double	m_dHeightWeb;			// 복부 높이
	double	m_dThickWeb;			// 복부 두께
	double	m_dSigma;				// 복부판 앞축응력
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
// 수평보강재
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
	double	m_dVerStiffSpace;		// 수직보강재의 간격
	double	m_dBhs;					// 수평보강재의 폭
	double	m_dThs;					// 수평보강재의 두께
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
// 단부 가로보(제작타입.H형강타입)
#if !defined(AFX_DESIGN_ENDCROSSBEAM)
#define AFX_DESIGN_ENDCROSSBEAM

class AFX_EXT_CLASS CDesignEndCrossBeam : public CObject
{
public:
	CDesignEndCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignEndCrossBeam();

	void SetDataInit();
	void SetGeneralType();

//-> 일반타입
	void SetJewon();				// 제원설정
	void SetVerLoad();				// 연직하중
	void SetHorLoad();				// 수평하중
	void SetEarthQuakeLoad();		// 지진하중
	void SetElementLoad();			// 부재력 계산
	void SetFrameElementLoad();		// Frame 해석결과
	void SetDesignSection();		// 단면설계
	void SetCompositeStress();		// 부재력의 조합및 응력검토
	void SetMinThick();

	long	m_nQtyHorStiff;						// 수평보강재 단수

	struct _MINTHICK							// 최소두께
	{
		double	m_dThickWebReq;					// 복부 필요두께
		double	m_dMinThickFlangeUp;			// 상부 플랜지 최소두께
		double	m_dMinThickFlangeLo;			// 하부 플랜지 최소두께
	} m_strtMinThick;

	struct _VERLOAD								// 연직하중
	{
		// 고정하중
		double	m_dGirderSpace;					// 주형간격
		double	m_dDistVerBracingToEndSlab;		// 수직브레이싱에서 상판단부까지 거리
		double	m_dLoadPave;					// 포장하중
		double	m_dLoadSlab;					// 상판하중
		double	m_dQ1;
		double	m_dQ2;
		// 활하중
		double	m_dCollisionCoeff;				// 충격계수(산식)
		double	m_dCollisionCoeffUse;			// 충격계수(사용)
	} m_strtVerLoad;

	struct _HORLOAD								// 수평하중
	{
		double	m_dWidthBridge;					// 교량폭원
		double	m_dHeightBridge;				// 교량높이
		double	m_dRateWidthHeight;
		double	m_dWw;							// 풍하중강도(산식)
		double	m_dWwUse;						// 풍하중강도(사용)
		double	m_dLengthSpan;					// 해당 지간길이
		double	m_dHw;							// 풍하중에 의한 브레이싱 1조의 작용력
	} m_strtHorLoad;

	struct _EARTHQUAKELOAD						// 지진하중
	{
		double	m_dReaction;					// 해당지점반력 합계
		double	m_dAcceleration;				// 지진가속도 계수
		double	m_dTotEarthQuakeLoad;			// 단부 브레이싱에 작용하는 총지진하중
		double	m_dEarthQuakeLoad;				// 지진하중에 의한 브레이싱 1조의 작용력
	} m_strtEarthQuakeLoad;

	struct _ELEMENTLOAD
	{
		double	m_dMd;							// 고정하중에 대한 휨모멘트
		double	m_dMl;							// 활하중에 대한 휨모멘트
		double	m_dSd;							// 고정하중에 대한 전단력
		double	m_dSl;							// 활하중에 대한 전단력
		double	m_dHw;							// 풍하중에 의한 수평력
		double	m_dHe;							// 지진하중 의한 수평력
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

	struct _FRAMELOADCOMPOSITE					// 부재력 조합및 응력검토
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
// 중간 가로보(제작타입.H형강타입)
#if !defined(AFX_DESIGN_CENTERCROSSBEAM)
#define AFX_DESIGN_CENTERCROSSBEAM
class AFX_EXT_CLASS CDesignCenterCrossBeam : public CObject
{
public:
	CDesignCenterCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignCenterCrossBeam();
	void SetDataInit();
	void SetGeneralType();

	void SetJewon();				// 제원설정
	void SetFrameElementLoad();		// Frame 해석결과
	void SetDesignSection();		// 단면설계
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

	struct _MINTHICK							// 최소두께
	{
		double	m_dThickWebReq;					// 복부 필요두께
		double	m_dMinThickFlangeUp;			// 상부 플랜지 최소두께
		double	m_dMinThickFlangeLo;			// 하부 플랜지 최소두께
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
// 단부 수직브레이싱(역V형 타입)
#if !defined(AFX_DESIGN_INVERVBRACING)
#define AFX_DESIGN_INVERVBRACING

class AFX_EXT_CLASS CDesignInverseVBracing : public CObject
{
public:
	CDesignInverseVBracing(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx);
	virtual ~CDesignInverseVBracing();
	void SetDataInit();
	void SetInverseVBracingType();
//-> 단부 수직브레이싱(역V형)
	void SetJewon();			// 사용강재
	void SetVerLoad();			// 연직하중
	void SetHorLoad();			// 수평하중
	void SetEarthQuakeLoad();	// 수평하중-지진하중
	void SetElementLoad();		// 부재력 계산 - 상현재.사재.하현재
	void SetDesignElement();	// 부재의 설계 - 상현재.사재.하현재
	void SetGussetPlate();		// Gusset Plate 검토 - 상현재.사재.하현재
	double	m_dGirderSpace;
	double	m_dHeight;
	double	m_dTheta;
	struct _VERLOAD								// 연직하중
	{
		// 고정하중
		double	m_dDistVerBracingToEndSlab;		// 수직브레이싱에서 상판단부까지 거리
		double	m_dLoadPave;					// 포장하중
		double	m_dLoadSlab;					// 상판하중
		double	m_dQ1;
		double	m_dQ2;
		double	m_dPd;
		// 활하중
		double	m_dLength;						// 활하중에 대한 종거
		double	m_dN1;
		double	m_dN2;
		double	m_dN3;
	} m_strtVerLoad;
	struct _HORLOAD								// 수평하중
	{
		double	m_dWidthBridge;					// 교량폭원
		double	m_dHeightBridge;				// 교량높이
		double	m_dRateWidthHeight;
		double	m_dWw;							// 풍하중강도(산식)
		double	m_dWwUse;						// 풍하중강도(사용)
		double	m_dLengthSpan;					// 해당 지간길이
		double	m_dHw;							// 풍하중에 의한 브레이싱 1조의 작용력
		double	m_dReaction;					// 해당지점반력 합계
		double	m_dAcceleration;				// 지진가속도 계수
		double	m_dTotEarthQuakeLoad;			// 단부 브레이싱에 작용하는 총지진하중
		double	m_dEarthQuakeLoad;				// 지진하중에 의한 브레이싱 1조의 작용력
		double	m_dH;
	} m_strtHorLoad;

	struct _ELEMENTLOAD
	{
		double	m_dCollCoefUp;					// 상현재 충격계수
		double	m_dCollCoefLo;					// 하현재 충격계수
		double	m_dCollCoefDiagonal;			// 사재 충격계수
		double	m_dN1;							// 상현재 수평하중재하시
		double	m_dMl;							// 상현재 연직 활하중 재하시
		double	m_dN2Hor;						// 사재 (수평하중만 작용시, BD부재 인장력 최대)
		double	m_dN2VerHor;					// 사재 (연직+수평하중 작용시, BE부재 압축력 최대)
		double	m_dN3Ver;						// 하현재 (연직하중만 작용시, 인장력 최대)
		double	m_dN3Hor;						// 하현재 (수평하중만 작용시, 압축력 최대)
	} m_strtElementLoad;

	struct _DESIGN_CHORDUP						// 상현재
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// 형강종류..CT형강이냐 아니냐...
		double	m_dSlendernessRatio;			// 세장비
		double	m_dFca;							// 허용 축방향 압축응력
		double	m_dFcaApo;						// 저감률고려한 허용합축응력
		double	m_dFa;
		double	m_dFc;
		double	m_dF;
		double	m_dAllowShear;					// Fillet 용접부의 허용응력
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dN1Apo;						// 
		double	m_dShear;
	} m_strtChordUp;

	struct _DESIGN_DIAGONAL						// 사재
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// 형강종류..CT형강이냐 아니냐...
		double	m_dSlendernessRatio;			// 세장비
		double	m_dFca;							// 허용 축방향 압축응력
		double	m_dFcaApo;						// 저감률고려한 허용합축응력
		double	m_dFta;							// 허용인장응력
		double	m_dAn;							// 유효단면적
		double	m_dFt;							//
		double	m_dFc;
		double	m_dAllowShear;					// Fillet 용접부의 허용응력
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dN2Apo;						// 
		double	m_dShear;
	} m_strtDiagonal;

	struct _DESIGN_CHORDLO						// 하현재
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// 형강종류..CT형강이냐 아니냐...
		double	m_dSlendernessRatio;			// 세장비
		double	m_dFca;							// 허용 축방향 압축응력
		double	m_dFcaApo;						// 저감률고려한 허용합축응력
		double	m_dFta;							// 허용인장응력
		double	m_dAn;							// 유효단면적
		double	m_dFt;							//
		double	m_dFc;
		double	m_dAllowShear;					// Fillet 용접부의 허용응력
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dN3Apo;						// 
		double	m_dShear;
	} m_strtChordLo;

	struct _GUSSETUP							// 상현재 연결
	{
		CString	m_strMaterial;
		double	m_dHeight;						// 세로
		double	m_dThick;						// 두께
		double	m_dWidth;						// 가로
		double	m_dAllowStress;					// 강재의 허용응력
		double	m_dLoad;						// 작용하중
		CString	m_strBoltMaterial;				// 볼트
		double	m_dDiaBolt;						// 볼트 직경
		double	m_dBoltAllowStress;				// 볼트 허용응력
		double	m_dQtyBoltReq;					// 필요볼트수
		double	m_dQtyBoltUse;					// 사용볼트수
		double	m_dAn;							// Gusset 순단면적
		double	m_dStress;						// 
	} m_strtGussetUp;

	struct _GUSSETDIAGONAL						// 사재/상현재 연결
	{
		CString	m_strMaterial;
		double	m_dHeight;						// 세로
		double	m_dThick;						// 두께
		double	m_dWidth;						// 가로
		double	m_dAllowStress;					// 강재의 허용응력
		double	m_dAllowShear;					// 강재의 허용전단응력
		double	m_dLoadHor;						// 작용하중
		double	m_dLoadVer;
		double	m_dLoad;
		double	m_dAllowShearWeld;
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dShearWeld;					// 용접부 전단력
		double	m_dAgv;
		double	m_dAgh;
		double	m_dStress;
		double	m_dShearGusset;
		double	m_dCompStress;
	} m_strtGussetDiagonal;

	struct _GUSSETLO							// 하현재/사재 연결
	{
		CString	m_strMaterial;
		double	m_dHeight;						// 세로
		double	m_dThick;						// 두께
		double	m_dWidth;						// 가로
		double	m_dAllowStress;					// 강재의 허용응력
		double	m_dRh;
		double	m_dRv;
		double	m_dLoad;						// 작용하중
		CString	m_strBoltMaterial;				// 볼트
		double	m_dDiaBolt;						// 볼트 직경
		double	m_dBoltAllowStress;				// 볼트 허용응력
		double	m_dQtyBoltReq;					// 필요볼트수
		double	m_dQtyBoltUse;					// 사용볼트수
		double	m_dAn;							// Gusset 순단면적
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
// 중간 수직브레이싱(V형 타입)
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

	//-> 중간 수직브레이싱(V형 타입)
	struct _WINDLOAD
	{
		double	m_dWidthBridge;					// 교량폭원
		double	m_dHeightBridge;				// 교량높이
		double	m_dRateWidthHeight;
		double	m_dWw;							// 풍하중의 강도
		double	m_dWwUse;						//
		double	m_dBracingSpace;				// 브레이싱 간격
		double	m_dAllowCoeff;					// 풍하중 계산시 허용응력 할증계수
		double	m_dHw;							// 풍하중에 의한 브레이싱 1조의 작용력
	} m_strtWind;

	struct _EARTHQUAKE
	{
		double	m_dReaction;					// 반력의 총합계
		double	m_dLengthBridge;				// 교량연장
		double	m_dWd;							// 단위길이당 고정하중
		double	m_dAcceleration;				// 지진가속도 계수
		double	m_dWe;							// 단위길이당 지진하중
		double	m_dHe;							// 지진하중에 의한 브레이싱 1조의 작용력
		double	m_dAllowCoeff;					// 지진하중 계산시 허용응력 할증계수
		double	m_dH;
	} m_strtEarthQuake;

	struct _ELEMENTLOAD
	{
		double	m_dN1;							// 상현재(AB) - 압축
		double	m_dN2Tension;					// 사재((AD:인장)
		double	m_dN2Comp;						// 사재((BD:압축)
		double	m_dN3;							// 하현재(CD) - 압축
	} m_strtElementLoad;

	struct _DESIGN_CHORDUP						// 상현재
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// 형강종류..CT형강이냐 아니냐...
		double	m_dSlendernessRatio;			// 세장비
		double	m_dFca;							// 허용 축방향 압축응력
		double	m_dFcaApo;						// 저감률고려한 허용합축응력
		double	m_dFc;
		double	m_dAllowShear;					// Fillet 용접부의 허용응력
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dN1Apo;						// 
		double	m_dShear;
	} m_strtChordUp;

	struct _DESIGN_DIAGONAL						// 사재
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// 형강종류..CT형강이냐 아니냐...
		double	m_dSlendernessRatio;			// 세장비
		double	m_dFca;							// 허용 축방향 압축응력
		double	m_dFcaApo;						// 저감률고려한 허용합축응력
		double	m_dFta;							// 허용인장응력
		double	m_dAn;							// 유효단면적
		double	m_dFt;							//
		double	m_dFc;
		double	m_dAllowShear;					// Fillet 용접부의 허용응력
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dN2Apo;						// 
		double	m_dShear;
	} m_strtDiagonal;

	struct _DESIGN_CHORDLO						// 하현재
	{
		CString	m_strMaterial;
		CString m_sMaterialType;				// 형강종류..CT형강이냐 아니냐...
		double	m_dSlendernessRatio;			// 세장비
		double	m_dFca;							// 허용 축방향 압축응력
		double	m_dFcaApo;						// 저감률고려한 허용합축응력
		double	m_dAn;							// 유효단면적
		double	m_dFc;
		double	m_dAllowShear;					// Fillet 용접부의 허용응력
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dN3Apo;						// 
		double	m_dShear;
	} m_strtChordLo;

	struct _GUSSETUP							// 상현재/사재 연결
	{
		CString	m_strMaterial;
		double	m_dHeight;						// 세로
		double	m_dThick;						// 두께
		double	m_dWidth;						// 가로
		double	m_dAllowStress;					// 강재의 허용응력
		double	m_dRh;
		double	m_dRv;
		double	m_dLoad;						// 작용하중
		CString	m_strBoltMaterial;				// 볼트
		double	m_dDiaBolt;						// 볼트 직경
		double	m_dBoltAllowStress;				// 볼트 허용응력
		double	m_dQtyBoltReq;					// 필요볼트수
		double	m_dQtyBoltUse;					// 사용볼트수
		double	m_dAn;							// Gusset 순단면적
		double	m_dStress;						// 
	} m_strtGussetUp;

	struct _GUSSETLO							// 하현재 연결
	{
		CString	m_strMaterial;
		double	m_dHeight;						// 세로
		double	m_dThick;						// 두께
		double	m_dWidth;						// 가로
		double	m_dAllowStress;					// 강재의 허용응력
		double	m_dLoad;						// 작용하중
		CString	m_strBoltMaterial;				// 볼트
		double	m_dDiaBolt;						// 볼트 직경
		double	m_dBoltAllowStress;				// 볼트 허용응력
		double	m_dQtyBoltReq;					// 필요볼트수
		double	m_dQtyBoltUse;					// 사용볼트수
		double	m_dAn;							// Gusset 순단면적
		double	m_dStress;						// 
	} m_strtGussetLo;

	struct _GUSSETDIAGONAL						// 사재/하현재 연결
	{
		CString	m_strMaterial;
		double	m_dHeight;						// 세로
		double	m_dThick;						// 두께
		double	m_dWidth;						// 가로
		double	m_dAllowStress;					// 강재의 허용응력
		double	m_dAllowShear;					// 강재의 허용전단응력
		double	m_dLoad;
		double	m_dAllowShearWeld;
		double	m_dLengthWeld;					// Fillet 용접각장
		double	m_dThroatDepth;					// Fillet 용접목두께
		double	m_dShearWeld;					// 용접부 전단력
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
// 가로보 이음
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
// 가로보 플랜지 이음
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

	struct _GUESSET								// 거세트방식
	{
		CString	m_strMaterial;
		CString m_sRightMaterial;				// 강종 오른쪽 이름
		double	m_dStress;						// 작용응력
		double	m_dAllowStress;					// 허용응력
		double  m_dDesignStress;					// 설계응력
		double	m_dAxialLoad;					// 설계축력
		double	m_dAreaFlange;					// 플랜지 단면적
		CString	m_strBoltMaterial;				// 볼트
		CString m_sLeftBoltMaterial;			//  F8T 에서 F8만.
		double	m_dDiaBolt;						// 볼트직경
		double	m_dAllowBolt;					// 볼트 1개당 허용응력
		long	m_nQtySide;						// 1면마찰, 2면마찰
		double	m_dBoltReq;						// 필요볼트수
		double	m_dBoltUse;						// 사용볼트수
		long	m_nQtyLine;						// 사용열수
		long	m_nTopConEA;					// 상면 연결판 갯수
		long	m_nBottomConEA;					// 하면 연결판 갯수
		double  m_dTopWidth;					// 상면 PL폭
		double  m_dBottomWidth;					// 하면 PL폭
		double  m_dTopThick;					// 상면 PL두께
		double  m_dBottomThick;					// 하면 PL두께
		double  m_dTR;							// 거셋판 중심 길이		
		double	m_dSumAs;						// 전단면적
		double	m_dAsn;							// 순단면적
		double  m_dConStress;					// 이음판 응력		
						
		double  m_dWidth;
		double  m_dThick;				
	} m_strtGusset;

	struct _PLATE
	{
		CString	m_strMaterial;
		double	m_dStress;						// 작용응력
		double	m_dAllowStress;					// 허용응력
		double	m_dDesignStress;				// 설계응력
		double	m_dThick;
		double	m_dWidth;
		double	m_dAf;							// 모재단면적
		CString	m_strBoltMaterial;				// 볼트 강종
		CString m_sLeftBoltMaterial;			//  F8T 에서 F8만.
		double	m_dDiaBolt;						// 볼트 직경
		double	m_dAllowBolt;					// 볼트 1개당 허용응력
		double	m_dQtyBoltReq;					// 필요볼트수
		double	m_dQtyBoltUse;					// 사용볼트수
		long	m_nQtyLine;						// 배치열수
		double	m_dTw;
		double	m_dL1;
		double	m_dA1;
		double	m_dA2;
		double	m_dA3;
		double	m_dTsMin;
		double	m_dTsUse;
		double	m_dTsCoeff;
		double	m_dSumAs;
		double	m_dAsn;							// 하부 순단면적
		double	m_dShear;						// 작용전단응력
		double  m_dQ1;
		double	m_dQ;							// 중립축에대한 접합선외측의 단면1차모멘트		
		double	m_dI;							// 중립축에대한 총단면의 단면2차모멘트
		double	m_dRhop;						// 직응력
		double	m_dRhoh;						// 휨전단응력
		double	m_dRho;							// 합성응력 검토
		double	m_dAreaFlange;					// 플랜지 총단면적
		double	m_dAsnFlange;					// 볼트구멍을 공제한 플랜지 순단면적
		double	m_dStressFlange;						// 모재의 응력
		long	m_nTopConEA;					// 상면 연결판 갯수
		long	m_nBottomConEA;					// 하면 연결판 갯수
	} m_strtPlate;

	long	m_nType;							// 거세트방식(일체형,분리형), 기존방식
	BOOL	m_bIsUpper;							// 상하부 플랜지
private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
	CPlateBasicIndex	*m_pBx;
};
#endif // !defined(AFX_DESIGN_CROSSBEAM_FLANGE_JOINT)

//////////////////////////////////////////////////////////////////////////
// 가로보 복부 이음
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

	struct _GUESSET								// 거세트방식
	{
		CString	m_strMaterial;
		double  m_dApplyLoad;		
		CString	m_strBoltMaterial;				// 볼트
		CString m_sLeftBoltMaterial;			//  F8T 에서 F8만.
		double	m_dDiaBolt;						// 볼트직경
		double	m_dAllowBolt;					// 볼트 1개당 허용응력
		double	m_dBoltReq;						// 필요볼트수
		double	m_dBoltUse;						// 사용볼트수
		long	m_nQtyLine;						// 사용열수	

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
		double	m_dRhom;						// 휨모멘트에 의한 보울트의 작용력 ( 2면마찰 )
		double	m_dRhos;						// 전단력에 의한 보울트의 작용력 ( 2면전단 )
		double	m_dRho;							// 합성응력의 검토
		double	m_dTsCoeff;
		double	m_dTsMin;
		double	m_dTsUse;
		double	m_dMoment;						// 작용 모멘트
		double	m_dIw;							// 복부만의 단면2차 모멘트
		double  m_dI;							// 세로보 전체의 단면2차모멘트
		double	m_dMw;							// 복부가 담당하는 휨모멘트
		double	m_Is;							// SPLICE의 단면2차 모멘트
		double	m_dFMax;						// SPLICE의 최대응력
		double  m_dReqArea;						// 필요단면적..
		double	m_dSumAs;
		double	m_dShear;
		double  m_dAllowTension;
		double	m_dAllowShear;
		double	m_dCompStress;
		CString	m_strMaterial;					// 강종
		double  m_dStress;						// 작용응력
		double  m_dA;
		double  m_dA2;
		double  m_dsE;
		double  m_dsF;
		double  m_dTop;
		double	m_dAllowBolt;					// 볼트 1개당 허용응력
		double	m_dDiaBolt;						// 볼트 직경
		double  m_dsT1;
		double  m_dsT2;
		double  m_dApplyLoad;					//작용전단력...
	} m_strtPlate;

	long	m_nType;							// 거세트방식, 기존방식
private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
	CPlateBasicIndex	*m_pBx;
};
#endif // !defined(AFX_DESIGN_CROSSBEAM_WEB_JOINT)

//////////////////////////////////////////////////////////////////////////
// 수평브레이싱
#if !defined(AFX_DESIGN_HOR_BRACING)
#define AFX_DESIGN_HOR_BRACING

class AFX_EXT_CLASS CDesignHorBracing : public CObject
{
public:
	CDesignHorBracing(CAPlateCalcStd *pAPlateCalcStd, long nG, long nIdxBracing);
	virtual ~CDesignHorBracing();

	void SetHorBracing();

	long	m_nG;
	long	m_nIndex;						// 수평브레이싱 Index
	double	m_dWindLoad;					// 풍하중
	double	m_dWw;							// 풍하중 강도
	double	m_dEarthQuakeLoad;				// 지진하중
	double	m_dWe;							// 지진하중 강도
	double	m_dLengthSection;				// 구간장
	double	m_dAreaPositiveInf;				// 영향선 면적(+)
	double	m_dAreaNegativeInf;				// 형향선 면적(-)
	double	m_dTheta;
	double	m_dLengthElem;					// 부재장
	double	m_dNwPositive;
	double	m_dNwNegative;
	double	m_dNe;
	CString	m_strMaterial;
	CString m_strMaterialType;
	double	m_dSlendernessRatio;			// 세장비
	double	m_dFca;							// 허용 축방향 압축응력
	double	m_dFcaApo;						// 저감률고려한 허용합축응력
	double	m_dFta;
	double	m_dAn;
	double	m_dFt;
	double	m_dFc;
	double	m_dDesignLoad;					// 설계작용력
	CString	m_strBoltMaterial;				// 볼트 강종
	double	m_dDiaBolt;						// 볼트 직경
	double	m_dBoltAllowStress;
	double	m_dQtyBoltReq;					// 필요볼트수
	double	m_dQtyBoltUse;					// 사용볼트수
	long	m_nQtyLine;						// 배치열수
	
	CBracingJewon		*m_pBracingJewon;
private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
};
#endif // !defined(AFX_DESIGN_HOR_BRACING)