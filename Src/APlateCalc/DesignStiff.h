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

// 지점부 보강재 - Web보강재(수평.수직)
class AFX_EXT_CLASS CDesignStiff : public CObject
{
public:
	CDesignStiff(CAPlateCalcStd *pAPlateCalcStd);
	virtual ~CDesignStiff();

	void SetVerStiff(CPlateBasicIndex *pBx);
	void SetHorStiff(CPlateBasicIndex *pBx);
	
	struct _VERSTIFF
	{
		CString	m_strMaterial;		//수직보강재 재질
		double	m_dGap;				//수직보강재 간격 a
		double	m_dMaxA_B;			//간격비 최대..
		double	m_dUseA_B;			//사용 간격비..
		double	m_dUseW;			//사용 수직보강재 폭 bvs
		double	m_dReqW;			//필요 수직보강재 폭
		double	m_dUseT;			//사용 수직보강재 두께 tvs		
		double	m_dReqT;			//필요 수직보강재 최소두께
		double	m_dFlangeGap;		//상하양플랜지의 순간격 b
		double	m_dWebThick;		//복부판의 두께 t
		double	m_dUseI;			//사용 강도..
		double	m_dReqI;			//필요 강도..
		
	} m_strtVerStiff;

	struct _HORSTIFF
	{
		CString	m_strMaterial;		//수평보강재 재질
		double	m_dGap;				//수직보강재 간격 a
		long	m_lUse_EA;			//수평보강재 사용 갯수
		double  m_dUseWebT;			//복부판 사용두께
		double  m_dReqWebT;			//복부판 필요두께..
		double	m_dUseW;			//사용 수직보강재 폭 bvs
		double	m_dReqW;			//필요 수직보강재 폭
		double	m_dUseT;			//사용 수평보강재 두께 tvs		
		double	m_dReqT;			//필요 수평보강재 최소두께
		double	m_dUseI;			//사용 강도..
		double	m_dReqI;			//필요 강도..
		CString m_sReqPlace;		//설치위치의 적정성..적정위치
		CString m_sUsePlase;		//       //        ..설치위치
		double	m_dFlangeGap;		//상하양플랜지의 순간격 b
		
	} m_strtHorStiff;

private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
};

#endif // !defined(AFX_DESIGNJIJUMSTIFF_H__DF7A5263_55A4_4A08_9989_CE35D78938C4__INCLUDED_)

// 지점부 보강재
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
		CString	m_strMaterial;		//보강재 재질
		double	m_dReaction;		//지점반력
		double  m_dWidth;			//폭
		double  m_dLength;			//길이
		double  m_dGap;				//설치간격
		double	m_dUseT;			//두께
		double  m_dReqT;			//필요 최소 두께
		double  m_dUse_EA;			//사용 열수
		double  m_dWebT;			//복부 두께
		double  m_dDanW_Be;			//단면유효폭
		double  m_dDanArea_As;		//단면적
		double  m_dReqArea_Ae;		//유효단면겆
		double  m_dMom_I;			//단면2차모멘트
		double  m_dDan_r;			//단면 2차 반경
		double  m_dBuckLing_le;		//유효좌굴길이
		double  m_dFcag;			//국부좌굴을 고려하지 않은 허용축방향 압축응력
		double  m_dFcal;			//국부좌굴에 대한 허용응력
		double  m_dFcao;			//국부좌굴을 고려하지 않은 최대 허용축방향 압축응력
		double  m_dFca;				//허용축방향 압축응력
		double  m_dFc;				//					
	} m_strJijumStiff;

	struct _JackupStiff
	{
		CString	m_strMaterial;		//잭업보강재 재질
		double	m_dReaction;		
		double  m_dWidthL;
		double  m_dWidthR;
		double  m_dLength;			//길이
		double  m_dGap;				//설치간격
		double	m_dUseT;			//두께
		double  m_dReqT;			//필요 최소 두께
		double  m_dUse_EA;			//사용 열수
		double  m_dQtyOneShoe;		//SHOE 1개소당 JACK UP장치 설치개소
		double  m_dWebT;			//복부 두께
		double  m_dDanW_Be;			//단면유효폭
		double  m_dDanArea_As;		//단면적
		double  m_dReqArea_Ae;		//유효단면겆
		double  m_dMom_I;			//단면2차모멘트
		double  m_dDan_r;			//단면 2차 반경
		double  m_dBuckLing_le;		//유효좌굴길이
		double  m_dFcag;			//국부좌굴을 고려하지 않은 허용축방향 압축응력
		double  m_dFcal;			//국부좌굴에 대한 허용응력
		double  m_dFcao;			//국부좌굴을 고려하지 않은 최대 허용축방향 압축응력
		double  m_dFca;				//허용축방향 압축응력
		double  m_dFc;				//	
		double  m_dApplyLoad_P;		//적용 하중...
		double  m_dPos_Yo;			//중립축의 위치

	} m_strJackupStiff;

private:
	CAPlateCalcStd		*m_pAPlateCalcStd;
	CPlateBasicIndex	*m_pBx;
	long				 m_nJoint;
};

#endif // !defined(AFX_DESIGN_JIJUM_STIFF)
