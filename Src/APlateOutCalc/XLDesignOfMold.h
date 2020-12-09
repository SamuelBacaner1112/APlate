// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignOfMold.h: interface for the CXLDesignOfMold class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNOFMOLD_H__BADE3CBC_9E0D_4ADB_800C_9A6D69D372A7__INCLUDED_)
#define AFX_XLDESIGNOFMOLD_H__BADE3CBC_9E0D_4ADB_800C_9A6D69D372A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateCrossBeam;
class CVBracing;
class CMovingLoad;

class CXLDesignOfMold  : public CAPlateOutCalcXL
{
public:
	CXLDesignOfMold(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignOfMold();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void CalcEffectWidth();					///< 4.1 유효폭 계산
	void SectionAssumptionGirder();			///< 4.2 단면가정 - 거더
	void SectionAssumptionGirderSub(long nG, double dSta, CFrameSection *pFrSec);		///< 4.2 단면가정 - 거더
	void SectionAssumptionCR();				///< 4.2 단면가정 - 가로보
	void SectionAssumptionST();				///< 4.2 단면가정 - 세로보
	void ComputeHLoad();					///< 4.3 작용하중 산정 - 가) 횡방향 하중분배
	void ComputeHLoad_Wind();				///< 4.3 작용하중 산정 - 풍하중
	void ComputeHLoad_Collision();			///< 4.3 작용하중 산정 - 충격계수
	void ComputeHLoad_Mold();				///< 4.3 작용하중 산정 - 나) 주형별 작용하중
	void ComputeHLoad_Mold_Cross();			///<  나) 주형별 작용하중 - 강재자중 가로보
	void ComputeHLoad_Mold_TU();			///<  나) 주형별 작용하중 - 구속콘크리트 자중, 180˚TURNING (TU거더)
	void Modeling();						///< 4.4 가) 모델링
	void SectionJewon();					///< 4.4 나) 단면제원
	void Load();							///< 4.4 다) 하중재하
	void EleForceTotal();					///< 4.4 라) 부재력 집계
	void EleForceLoadType();				///< 4.4 마) 작용하중별 부재력 집계
	void EleForceSummary();					///< 4.4 바) 부재력 요약
	void EleForceReaction();				///< 4.4 사) 반 력 집 계
	void WarpingTorsion();					///< 4.5 뒴비틀림
	void InfluenceK();						///< 4.6 2차응력 - 영향계수 K
	CString RetMaxString(CString szStr);
protected:
	long m_nNumber;
	long m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	CString				m_strCell[50];
	CStringArray		m_szArrSecJointNum;

	double GetScaceFactor(long nLane);

	void SetLines(int nLine);
	void CalcFlangeEffectWidth();
	void CalcSlabEffectWidth();

	void TypeCrossBeam(CPlateBasicIndex *pBx, CPlateCrossBeam *pCrossBeam);
	void TypeVBracing(CPlateBasicIndex *pBx, CVBracing *pVBracing);

	void EleForceSetTable(long nForceType);

	///< 4.3 작용하중의 산정 - 나) 주형별 작용하중
	///< 2) 합성전 고정하중 3) 합성후 고정하중
	void ComputeHLoad_DeadLoad();
	void ComputeHLoad_LiveLoad();
	void ComputeHLoad_PeopleLoad();
	void ComputeHLoad_WindLoad();
	///< 영향계수 그림
	void DrawInfK(CDomyun* pDom, long nG);
	void DrawHLoad(CDomyun* pDom, CFEMManage *pFEM, long nLoadCase);

	CString GetJointArrszByI33(double dI33, long nType, long nG = -1);

	void ModelPlanOutput(CDomyun *pDom);
	void CalcNodeCoord(CDPointArray &pNode, double &dShoeLength, double dWidth);
	CDPoint GetCenterOffsetPos(double sta, double dOffset, long nG=-1);
	void WriteLoadForce(CFEMManage *pFEM, long nLoadCase);
private:
	BOOL IsOuterGirder(long nG);
};

#endif // !defined(AFX_XLDESIGNOFMOLD_H__BADE3CBC_9E0D_4ADB_800C_9A6D69D372A7__INCLUDED_)
